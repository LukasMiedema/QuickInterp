/*
 * Copyright (c) 2002, 2018, Oracle and/or its affiliates. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Oracle, 500 Oracle Parkway, Redwood Shores, CA 94065 USA
 * or visit www.oracle.com if you need additional information or have any
 * questions.
 *
 */

// no precompiled headers
#include "classfile/vmSymbols.hpp"
#include "gc/shared/collectedHeap.hpp"
#include "gc/shared/threadLocalAllocBuffer.inline.hpp"
#include "interpreter/bytecodeHistogram.hpp"
#include "interpreter/bytecodeInterpreter.hpp"
#include "interpreter/bytecodeInterpreter.inline.hpp"
#include "interpreter/bytecodeInterpreterProfiling.hpp"
#include "interpreter/interpreter.hpp"
#include "interpreter/interpreterRuntime.hpp"
#include "logging/log.hpp"
#include "memory/resourceArea.hpp"
#include "oops/constantPool.inline.hpp"
#include "oops/cpCache.inline.hpp"
#include "oops/method.inline.hpp"
#include "oops/methodCounters.hpp"
#include "oops/objArrayKlass.hpp"
#include "oops/objArrayOop.inline.hpp"
#include "oops/oop.inline.hpp"
#include "oops/typeArrayOop.inline.hpp"
#include "prims/jvmtiExport.hpp"
#include "prims/jvmtiThreadState.hpp"
#include "runtime/atomic.hpp"
#include "runtime/biasedLocking.hpp"
#include "runtime/frame.inline.hpp"
#include "runtime/handles.inline.hpp"
#include "runtime/interfaceSupport.inline.hpp"
#include "runtime/orderAccess.hpp"
#include "runtime/sharedRuntime.hpp"
#include "runtime/threadCritical.hpp"
#include "utilities/exceptions.hpp"

// no precompiled headers
#ifdef CC_INTERP

/*
 * USELABELS - If using GCC, then use labels for the opcode dispatching
 * rather -then a switch statement. This improves performance because it
 * gives us the opportunity to have the instructions that calculate the
 * next opcode to jump to be intermixed with the rest of the instructions
 * that implement the opcode (see UPDATE_PC_AND_TOS_AND_CONTINUE macro).
 */
#undef USELABELS
#ifdef __GNUC__
/*
   ASSERT signifies debugging. It is much easier to step thru bytecodes if we
   don't use the computed goto approach.
*/
#ifndef ASSERT
#define USELABELS
#endif
#endif

#undef CASE

/*
 * PREFETCH_OPCCODE - Some compilers do better if you prefetch the next
 * opcode before going back to the top of the while loop, rather then having
 * the top of the while loop handle it. This provides a better opportunity
 * for instruction scheduling. Some compilers just do this prefetch
 * automatically. Some actually end up with worse performance if you
 * force the prefetch. Solaris gcc seems to do better, but cc does worse.
 */
#undef PREFETCH_OPCCODE
#define PREFETCH_OPCCODE

/*
  Interpreter safepoint: it is expected that the interpreter will have no live
  handles of its own creation live at an interpreter safepoint. Therefore we
  run a HandleMarkCleaner and trash all handles allocated in the call chain
  since the JavaCalls::call_helper invocation that initiated the chain.
  There really shouldn't be any handles remaining to trash but this is cheap
  in relation to a safepoint.
*/
#define SAFEPOINT                                                                 \
    {                                                                             \
       /* zap freed handles rather than GC'ing them */                            \
       HandleMarkCleaner __hmc(THREAD);                                           \
       CALL_VM(SafepointMechanism::block_if_requested(THREAD), handle_exception); \
    }

/*
 * VM_JAVA_ERROR - Macro for throwing a java exception from
 * the interpreter loop. Should really be a CALL_VM but there
 * is no entry point to do the transition to vm so we just
 * do it by hand here.
 */
#define VM_JAVA_ERROR_NO_JUMP(name, msg, note_a_trap)                             \
    DECACHE_STATE();                                                              \
    SET_LAST_JAVA_FRAME();                                                        \
    {                                                                             \
       InterpreterRuntime::note_a_trap(THREAD, istate->method(), BCI());          \
       ThreadInVMfromJava trans(THREAD);                                          \
       Exceptions::_throw_msg(THREAD, __FILE__, __LINE__, name, msg);             \
    }                                                                             \
    RESET_LAST_JAVA_FRAME();                                                      \
    CACHE_STATE();

// Normal throw of a java error.
#define VM_JAVA_ERROR(name, msg, note_a_trap)                                     \
    VM_JAVA_ERROR_NO_JUMP(name, msg, note_a_trap)                                 \
    goto handle_exception;

#ifdef PRODUCT
#define DO_UPDATE_INSTRUCTION_COUNT(opcode)
#else
#define DO_UPDATE_INSTRUCTION_COUNT(opcode)                                                          \
{                                                                                                    \
    BytecodeCounter::_counter_value++;                                                               \
    BytecodeHistogram::_counters[(Bytecodes::Code)opcode]++;                                         \
    if (StopInterpreterAt && StopInterpreterAt == BytecodeCounter::_counter_value) os::breakpoint(); \
    if (TraceBytecodes) {                                                                            \
      CALL_VM((void)InterpreterRuntime::trace_bytecode(THREAD, 0,                    \
                                        topOfStack[Interpreter::expr_index_at(1)],   \
                                        topOfStack[Interpreter::expr_index_at(2)]),  \
                                        handle_exception);                           \
    }                                                                                \
}
#endif

#undef DEBUGGER_SINGLE_STEP_NOTIFY
#ifdef VM_JVMTI
/* NOTE: (kbr) This macro must be called AFTER the PC has been
   incremented. JvmtiExport::at_single_stepping_point() may cause a
   breakpoint opcode to get inserted at the current PC to allow the
   debugger to coalesce single-step events.

   As a result if we call at_single_stepping_point() we refetch opcode
   to get the current opcode. This will override any other prefetching
   that might have occurred.
*/
#define DEBUGGER_SINGLE_STEP_NOTIFY()                                            \
{                                                                                \
      if (_jvmti_interp_events) {                                                \
        if (JvmtiExport::should_post_single_step()) {                            \
          DECACHE_STATE();                                                       \
          SET_LAST_JAVA_FRAME();                                                 \
          ThreadInVMfromJava trans(THREAD);                                      \
          JvmtiExport::at_single_stepping_point(THREAD,                          \
                                          istate->method(),                      \
                                          pc);                                   \
          RESET_LAST_JAVA_FRAME();                                               \
          CACHE_STATE();                                                         \
          if (THREAD->pop_frame_pending() &&                                     \
              !THREAD->pop_frame_in_process()) {                                 \
            goto handle_Pop_Frame;                                               \
          }                                                                      \
          if (THREAD->jvmti_thread_state() &&                                    \
              THREAD->jvmti_thread_state()->is_earlyret_pending()) {             \
            goto handle_Early_Return;                                            \
          }                                                                      \
          opcode = READ_BYTECODE(pc);                                                          \
        }                                                                        \
      }                                                                          \
}
#else
#define DEBUGGER_SINGLE_STEP_NOTIFY()
#endif

/*
 * CONTINUE - Macro for executing the next opcode.
 */
#undef CONTINUE
#ifdef USELABELS
// Have to do this dispatch this way in C++ because otherwise gcc complains about crossing an
// initialization (which is is the initialization of the table pointer...)
#define DISPATCH(opcode) goto *(void*)dispatch_table[opcode]
#define CONTINUE {                              \
        opcode = READ_BYTECODE(pc);                           \
        DO_UPDATE_INSTRUCTION_COUNT(opcode);    \
        DEBUGGER_SINGLE_STEP_NOTIFY();          \
        DISPATCH(opcode);                       \
    }
#else
#ifdef PREFETCH_OPCCODE
#define CONTINUE {                              \
        opcode = READ_BYTECODE(pc);                           \
        DO_UPDATE_INSTRUCTION_COUNT(opcode);    \
        DEBUGGER_SINGLE_STEP_NOTIFY();          \
        continue;                               \
    }
#else
#define CONTINUE {                              \
        DO_UPDATE_INSTRUCTION_COUNT(opcode);    \
        DEBUGGER_SINGLE_STEP_NOTIFY();          \
        continue;                               \
    }
#endif
#endif


#define UPDATE_PC(opsize) {pc += opsize; }
/*
 * UPDATE_PC_AND_TOS - Macro for updating the pc and topOfStack.
 */
#undef SET_PC_AND_TOS
#undef UPDATE_PC_AND_TOS

#define SET_PC_AND_TOS(new_pc, stack) \
    {pc = new_pc; MORE_STACK(stack); }
#define UPDATE_PC_AND_TOS(opsize, stack) \
    {pc += opsize; MORE_STACK(stack); }

/*
 * UPDATE_PC_AND_TOS_AND_CONTINUE - Macro for updating the pc and topOfStack,
 * and executing the next opcode. It's somewhat similar to the combination
 * of UPDATE_PC_AND_TOS and CONTINUE, but with some minor optimizations.
 */
#undef UPDATE_PC_AND_TOS_AND_CONTINUE
#ifdef USELABELS
#define UPDATE_PC_AND_TOS_AND_CONTINUE(opsize, stack) {         \
        pc += opsize; opcode = READ_BYTECODE(pc); MORE_STACK(stack);          \
        DO_UPDATE_INSTRUCTION_COUNT(opcode);                    \
        DEBUGGER_SINGLE_STEP_NOTIFY();                          \
        DISPATCH(opcode);                                       \
    }

#define UPDATE_PC_AND_CONTINUE(opsize) {                        \
        pc += opsize; opcode = READ_BYTECODE(pc);               \
        DO_UPDATE_INSTRUCTION_COUNT(opcode);                    \
        DEBUGGER_SINGLE_STEP_NOTIFY();                          \
        DISPATCH(opcode);                                       \
    }
#else
#ifdef PREFETCH_OPCCODE
#define UPDATE_PC_AND_TOS_AND_CONTINUE(opsize, stack) {         \
        pc += opsize; opcode = READ_BYTECODE(pc); MORE_STACK(stack);          \
        DO_UPDATE_INSTRUCTION_COUNT(opcode);                    \
        DEBUGGER_SINGLE_STEP_NOTIFY();                          \
        goto do_continue;                                       \
    }

#define UPDATE_PC_AND_CONTINUE(opsize) {                        \
        pc += opsize; opcode = READ_BYTECODE(pc);               \
        DO_UPDATE_INSTRUCTION_COUNT(opcode);                    \
        DEBUGGER_SINGLE_STEP_NOTIFY();                          \
        goto do_continue;                                       \
    }
#else
#define UPDATE_PC_AND_TOS_AND_CONTINUE(opsize, stack) { \
        pc += opsize; MORE_STACK(stack);                \
        DO_UPDATE_INSTRUCTION_COUNT(opcode);            \
        DEBUGGER_SINGLE_STEP_NOTIFY();                  \
        goto do_continue;                               \
    }

#define UPDATE_PC_AND_CONTINUE(opsize) {                \
        pc += opsize;                                   \
        DO_UPDATE_INSTRUCTION_COUNT(opcode);            \
        DEBUGGER_SINGLE_STEP_NOTIFY();                  \
        goto do_continue;                               \
    }
#endif /* PREFETCH_OPCCODE */
#endif /* USELABELS */

// About to call a new method, update the save the adjusted pc and return to frame manager
#define UPDATE_PC_AND_RETURN(opsize)  \
   DECACHE_TOS();                     \
   istate->set_bcp(pc+opsize);        \
   return;


#define METHOD istate->method()
#define GET_METHOD_COUNTERS(res)    \
  res = METHOD->method_counters();  \
  if (res == NULL) {                \
    CALL_VM(res = InterpreterRuntime::build_method_counters(THREAD, METHOD), handle_exception); \
  }

#define OSR_REQUEST(res, branch_pc) \
            CALL_VM(res=InterpreterRuntime::frequency_counter_overflow(THREAD, branch_pc), handle_exception);
/*
 * For those opcodes that need to have a GC point on a backwards branch
 */

// Backedge counting is kind of strange. The asm interpreter will increment
// the backedge counter as a separate counter but it does it's comparisons
// to the sum (scaled) of invocation counter and backedge count to make
// a decision. Seems kind of odd to sum them together like that

// skip is delta from current bcp/bci for target, branch_pc is pre-branch bcp


#define DO_BACKEDGE_CHECKS(skip, branch_pc)                                                         \
    if ((skip) <= 0) {                                                                              \
      MethodCounters* mcs;                                                                          \
      GET_METHOD_COUNTERS(mcs);                                                                     \
      if (UseLoopCounter) {                                                                         \
        bool do_OSR = UseOnStackReplacement;                                                        \
        mcs->backedge_counter()->increment();                                                       \
        if (ProfileInterpreter) {                                                                   \
          BI_PROFILE_GET_OR_CREATE_METHOD_DATA(handle_exception);                                   \
          /* Check for overflow against MDO count. */                                               \
          do_OSR = do_OSR                                                                           \
            && (mdo_last_branch_taken_count >= (uint)InvocationCounter::InterpreterBackwardBranchLimit)\
            /* When ProfileInterpreter is on, the backedge_count comes     */                       \
            /* from the methodDataOop, which value does not get reset on   */                       \
            /* the call to frequency_counter_overflow(). To avoid          */                       \
            /* excessive calls to the overflow routine while the method is */                       \
            /* being compiled, add a second test to make sure the overflow */                       \
            /* function is called only once every overflow_frequency.      */                       \
            && (!(mdo_last_branch_taken_count & 1023));                                             \
        } else {                                                                                    \
          /* check for overflow of backedge counter */                                              \
          do_OSR = do_OSR                                                                           \
            && mcs->invocation_counter()->reached_InvocationLimit(mcs->backedge_counter());         \
        }                                                                                           \
        if (do_OSR) {                                                                               \
          nmethod* osr_nmethod;                                                                     \
          OSR_REQUEST(osr_nmethod, branch_pc);                                                      \
          if (osr_nmethod != NULL && osr_nmethod->is_in_use()) {                                    \
            intptr_t* buf;                                                                          \
            /* Call OSR migration with last java frame only, no checks. */                          \
            CALL_VM_NAKED_LJF(buf=SharedRuntime::OSR_migration_begin(THREAD));                      \
            istate->set_msg(do_osr);                                                                \
            istate->set_osr_buf((address)buf);                                                      \
            istate->set_osr_entry(osr_nmethod->osr_entry());                                        \
            return;                                                                                 \
          }                                                                                         \
        }                                                                                           \
      }  /* UseCompiler ... */                                                                      \
      SAFEPOINT;                                                                                    \
    }

/*
 * For those opcodes that need to have a GC point on a backwards branch
 */

/*
 * Macros for caching and flushing the interpreter state. Some local
 * variables need to be flushed out to the frame before we do certain
 * things (like pushing frames or becomming gc safe) and some need to
 * be recached later (like after popping a frame). We could use one
 * macro to cache or decache everything, but this would be less then
 * optimal because we don't always need to cache or decache everything
 * because some things we know are already cached or decached.
 */
#undef DECACHE_TOS
#undef CACHE_TOS
#undef CACHE_PREV_TOS
#define DECACHE_TOS()    istate->set_stack(topOfStack);

#define CACHE_TOS()      topOfStack = (intptr_t *)istate->stack();

#undef DECACHE_PC
#undef CACHE_PC
#define DECACHE_PC()    istate->set_bcp(pc);
#define CACHE_PC()      pc = istate->bcp();
#define CACHE_CP()      cp = istate->constants();
#define CACHE_LOCALS()  locals = istate->locals();
#undef CACHE_FRAME
#define CACHE_FRAME()

// BCI() returns the current bytecode-index.
#undef  BCI
#define BCI()           ((int)(intptr_t)(pc - (intptr_t)istate->method()->code_base()))

/*
 * CHECK_NULL - Macro for throwing a NullPointerException if the object
 * passed is a null ref.
 * On some architectures/platforms it should be possible to do this implicitly
 */
#undef CHECK_NULL
#define CHECK_NULL(obj_)                                                                         \
        if ((obj_) == NULL) {                                                                    \
          VM_JAVA_ERROR(vmSymbols::java_lang_NullPointerException(), NULL, note_nullCheck_trap); \
        }                                                                                        \
        VERIFY_OOP(obj_)

#define VMdoubleConstZero() 0.0
#define VMdoubleConstOne() 1.0
#define VMlongConstZero() (max_jlong-max_jlong)
#define VMlongConstOne() ((max_jlong-max_jlong)+1)

/*
 * Alignment
 */
#define VMalignWordUp(val)          (((uintptr_t)(val) + 3) & ~3)

// Decache the interpreter state that interpreter modifies directly (i.e. GC is indirect mod)
#define DECACHE_STATE() DECACHE_PC(); DECACHE_TOS();

// Reload interpreter state after calling the VM or a possible GC
#define CACHE_STATE()   \
        CACHE_TOS();    \
        CACHE_PC();     \
        CACHE_CP();     \
        CACHE_LOCALS();

// Call the VM with last java frame only.
#define CALL_VM_NAKED_LJF(func)                                    \
        DECACHE_STATE();                                           \
        SET_LAST_JAVA_FRAME();                                     \
        func;                                                      \
        RESET_LAST_JAVA_FRAME();                                   \
        CACHE_STATE();

// Call the VM. Don't check for pending exceptions.
#define CALL_VM_NOCHECK(func)                                      \
        CALL_VM_NAKED_LJF(func)                                    \
        if (THREAD->pop_frame_pending() &&                         \
            !THREAD->pop_frame_in_process()) {                     \
          goto handle_Pop_Frame;                                   \
        }                                                          \
        if (THREAD->jvmti_thread_state() &&                        \
            THREAD->jvmti_thread_state()->is_earlyret_pending()) { \
          goto handle_Early_Return;                                \
        }

// Call the VM and check for pending exceptions
#define CALL_VM(func, label) {                                     \
          CALL_VM_NOCHECK(func);                                   \
          if (THREAD->has_pending_exception()) goto label;         \
        }

/*
 * BytecodeInterpreter::run(interpreterState istate)
 * BytecodeInterpreter::runWithChecks(interpreterState istate)
 *
 * The real deal. This is where byte codes actually get interpreted.
 * Basically it's a big while loop that iterates until we return from
 * the method passed in.
 *
 * The runWithChecks is used if JVMTI is enabled.
 *
 */
#if defined(VM_JVMTI)
void
BytecodeInterpreter::runWithChecks(interpreterState istate) {
#else
void
BytecodeInterpreter::run(interpreterState istate) {
#endif

  // In order to simplify some tests based on switches set at runtime
  // we invoke the interpreter a single time after switches are enabled
  // and set simpler to to test variables rather than method calls or complex
  // boolean expressions.

  static int initialized = 0;
  static int checkit = 0;
  static intptr_t* c_addr = NULL;
  static intptr_t  c_value;

  if (checkit && *c_addr != c_value) {
    os::breakpoint();
  }
#ifdef VM_JVMTI
  static bool _jvmti_interp_events = 0;
#endif

  static int _compiling;  // (UseCompiler || CountCompiledCalls)

#ifdef ASSERT
  if (istate->_msg != initialize) {
    assert(labs(istate->_stack_base - istate->_stack_limit) == (istate->_method->max_stack() + 1), "bad stack limit");
    IA32_ONLY(assert(istate->_stack_limit == istate->_thread->last_Java_sp() + 1, "wrong"));
  }
  // Verify linkages.
  interpreterState l = istate;
  do {
    assert(l == l->_self_link, "bad link");
    l = l->_prev_link;
  } while (l != NULL);
  // Screwups with stack management usually cause us to overwrite istate
  // save a copy so we can verify it.
  interpreterState orig = istate;
#endif

  register intptr_t*        topOfStack = (intptr_t *)istate->stack(); /* access with STACK macros */
  register address          pc = istate->bcp();
  register jushort          opcode;
  register intptr_t*        locals = istate->locals();
  register ConstantPoolCache*    cp = istate->constants(); // method()->constants()->cache()
#ifdef LOTS_OF_REGS
  register JavaThread*      THREAD = istate->thread();
#else
#undef THREAD
#define THREAD istate->thread()
#endif

#ifdef USELABELS
  const static void* const opclabels_data[1 << 16] = {
    #include "generated/bytecodeInterpreter.jumptable.hpp"
  };
  register uintptr_t *dispatch_table = (uintptr_t*)&opclabels_data[0];
#endif /* USELABELS */

#ifdef ASSERT
  // this will trigger a VERIFY_OOP on entry
  if (istate->msg() != initialize && ! METHOD->is_static()) {
    oop rcvr = LOCALS_OBJECT(0);
    VERIFY_OOP(rcvr);
  }
#endif

  /* QQQ this should be a stack method so we don't know actual direction */
  guarantee(istate->msg() == initialize ||
         topOfStack >= istate->stack_limit() &&
         topOfStack < istate->stack_base(),
         "Stack top out of range");

#ifdef CC_INTERP_PROFILE
  // MethodData's last branch taken count.
  uint mdo_last_branch_taken_count = 0;
#else
  const uint mdo_last_branch_taken_count = 0;
#endif

  switch (istate->msg()) {
    case initialize: {
      if (initialized++) ShouldNotReachHere(); // Only one initialize call.
      _compiling = (UseCompiler || CountCompiledCalls);
#ifdef VM_JVMTI
      _jvmti_interp_events = JvmtiExport::can_post_interpreter_events();
#endif
      return;
    }
    break;
    case method_entry: {
      THREAD->set_do_not_unlock();
      // count invocations
      assert(initialized, "Interpreter not initialized");
      if (_compiling) {
        MethodCounters* mcs;
        GET_METHOD_COUNTERS(mcs);
#if COMPILER2_OR_JVMCI
        if (ProfileInterpreter) {
          METHOD->increment_interpreter_invocation_count(THREAD);
        }
#endif
        mcs->invocation_counter()->increment();
        if (mcs->invocation_counter()->reached_InvocationLimit(mcs->backedge_counter())) {
          CALL_VM((void)InterpreterRuntime::frequency_counter_overflow(THREAD, NULL), handle_exception);
          // We no longer retry on a counter overflow.
        }
        // Get or create profile data. Check for pending (async) exceptions.
        BI_PROFILE_GET_OR_CREATE_METHOD_DATA(handle_exception);
        SAFEPOINT;
      }

      if ((istate->_stack_base - istate->_stack_limit) != istate->method()->max_stack() + 1) {
        // initialize
        os::breakpoint();
      }

      // Lock method if synchronized.
      if (METHOD->is_synchronized()) {
        // oop rcvr = locals[0].j.r;
        oop rcvr;
        if (METHOD->is_static()) {
          rcvr = METHOD->constants()->pool_holder()->java_mirror();
        } else {
          rcvr = LOCALS_OBJECT(0);
          VERIFY_OOP(rcvr);
        }
        // The initial monitor is ours for the taking.
        // Monitor not filled in frame manager any longer as this caused race condition with biased locking.
        BasicObjectLock* mon = &istate->monitor_base()[-1];
        mon->set_obj(rcvr);
        bool success = false;
        uintptr_t epoch_mask_in_place = (uintptr_t)markOopDesc::epoch_mask_in_place;
        markOop mark = rcvr->mark();
        intptr_t hash = (intptr_t) markOopDesc::no_hash;
        // Implies UseBiasedLocking.
        if (mark->has_bias_pattern()) {
          uintptr_t thread_ident;
          uintptr_t anticipated_bias_locking_value;
          thread_ident = (uintptr_t)istate->thread();
          anticipated_bias_locking_value =
            (((uintptr_t)rcvr->klass()->prototype_header() | thread_ident) ^ (uintptr_t)mark) &
            ~((uintptr_t) markOopDesc::age_mask_in_place);

          if (anticipated_bias_locking_value == 0) {
            // Already biased towards this thread, nothing to do.
            if (PrintBiasedLockingStatistics) {
              (* BiasedLocking::biased_lock_entry_count_addr())++;
            }
            success = true;
          } else if ((anticipated_bias_locking_value & markOopDesc::biased_lock_mask_in_place) != 0) {
            // Try to revoke bias.
            markOop header = rcvr->klass()->prototype_header();
            if (hash != markOopDesc::no_hash) {
              header = header->copy_set_hash(hash);
            }
            if (rcvr->cas_set_mark(header, mark) == mark) {
              if (PrintBiasedLockingStatistics)
                (*BiasedLocking::revoked_lock_entry_count_addr())++;
            }
          } else if ((anticipated_bias_locking_value & epoch_mask_in_place) != 0) {
            // Try to rebias.
            markOop new_header = (markOop) ( (intptr_t) rcvr->klass()->prototype_header() | thread_ident);
            if (hash != markOopDesc::no_hash) {
              new_header = new_header->copy_set_hash(hash);
            }
            if (rcvr->cas_set_mark(new_header, mark) == mark) {
              if (PrintBiasedLockingStatistics) {
                (* BiasedLocking::rebiased_lock_entry_count_addr())++;
              }
            } else {
              CALL_VM(InterpreterRuntime::monitorenter(THREAD, mon), handle_exception);
            }
            success = true;
          } else {
            // Try to bias towards thread in case object is anonymously biased.
            markOop header = (markOop) ((uintptr_t) mark &
                                        ((uintptr_t)markOopDesc::biased_lock_mask_in_place |
                                         (uintptr_t)markOopDesc::age_mask_in_place | epoch_mask_in_place));
            if (hash != markOopDesc::no_hash) {
              header = header->copy_set_hash(hash);
            }
            markOop new_header = (markOop) ((uintptr_t) header | thread_ident);
            // Debugging hint.
            DEBUG_ONLY(mon->lock()->set_displaced_header((markOop) (uintptr_t) 0xdeaddead);)
            if (rcvr->cas_set_mark(new_header, header) == header) {
              if (PrintBiasedLockingStatistics) {
                (* BiasedLocking::anonymously_biased_lock_entry_count_addr())++;
              }
            } else {
              CALL_VM(InterpreterRuntime::monitorenter(THREAD, mon), handle_exception);
            }
            success = true;
          }
        }

        // Traditional lightweight locking.
        if (!success) {
          markOop displaced = rcvr->mark()->set_unlocked();
          mon->lock()->set_displaced_header(displaced);
          bool call_vm = UseHeavyMonitors;
          if (call_vm || rcvr->cas_set_mark((markOop)mon, displaced) != displaced) {
            // Is it simple recursive case?
            if (!call_vm && THREAD->is_lock_owned((address) displaced->clear_lock_bits())) {
              mon->lock()->set_displaced_header(NULL);
            } else {
              CALL_VM(InterpreterRuntime::monitorenter(THREAD, mon), handle_exception);
            }
          }
        }
      }
      THREAD->clr_do_not_unlock();

      // Notify jvmti
#ifdef VM_JVMTI
      if (_jvmti_interp_events) {
        // Whenever JVMTI puts a thread in interp_only_mode, method
        // entry/exit events are sent for that thread to track stack depth.
        if (THREAD->is_interp_only_mode()) {
          CALL_VM(InterpreterRuntime::post_method_entry(THREAD),
                  handle_exception);
        }
      }
#endif /* VM_JVMTI */

      goto run;
    }

    case popping_frame: {
      // returned from a java call to pop the frame, restart the call
      // clear the message so we don't confuse ourselves later
      assert(THREAD->pop_frame_in_process(), "wrong frame pop state");
      istate->set_msg(no_request);
      if (_compiling) {
        // Set MDX back to the ProfileData of the invoke bytecode that will be
        // restarted.
        SET_MDX(NULL);
        BI_PROFILE_GET_OR_CREATE_METHOD_DATA(handle_exception);
      }
      THREAD->clr_pop_frame_in_process();
      goto run;
    }

    case method_resume: {
      if ((istate->_stack_base - istate->_stack_limit) != istate->method()->max_stack() + 1) {
        // resume
        os::breakpoint();
      }
      // returned from a java call, continue executing.
      if (THREAD->pop_frame_pending() && !THREAD->pop_frame_in_process()) {
        goto handle_Pop_Frame;
      }
      if (THREAD->jvmti_thread_state() &&
          THREAD->jvmti_thread_state()->is_earlyret_pending()) {
        goto handle_Early_Return;
      }

      if (THREAD->has_pending_exception()) goto handle_exception;
      // Update the pc by the saved amount of the invoke bytecode size
      UPDATE_PC(istate->bcp_advance());

      if (_compiling) {
        // Get or create profile data. Check for pending (async) exceptions.
        BI_PROFILE_GET_OR_CREATE_METHOD_DATA(handle_exception);
      }
      goto run;
    }

    case deopt_resume2: {
      // Returned from an opcode that will reexecute. Deopt was
      // a result of a PopFrame request.
      //

      if (_compiling) {
        // Get or create profile data. Check for pending (async) exceptions.
        BI_PROFILE_GET_OR_CREATE_METHOD_DATA(handle_exception);
      }
      goto run;
    }

    case deopt_resume: {
      // Returned from an opcode that has completed. The stack has
      // the result all we need to do is skip across the bytecode
      // and continue (assuming there is no exception pending)
      //
      // compute continuation length
      //
      // Note: it is possible to deopt at a return_register_finalizer opcode
      // because this requires entering the vm to do the registering. While the
      // opcode is complete we can't advance because there are no more opcodes
      // much like trying to deopt at a poll return. In that has we simply
      // get out of here
      //
      if ( Bytecodes::code_at(METHOD, pc) == Bytecodes::_return_register_finalizer) {
        // this will do the right thing even if an exception is pending.
        goto handle_return;
      }
      UPDATE_PC(Bytecodes::length_at(METHOD, pc));
      if (THREAD->has_pending_exception()) goto handle_exception;

      if (_compiling) {
        // Get or create profile data. Check for pending (async) exceptions.
        BI_PROFILE_GET_OR_CREATE_METHOD_DATA(handle_exception);
      }
      goto run;
    }
    case got_monitors: {
      // continue locking now that we have a monitor to use
      // we expect to find newly allocated monitor at the "top" of the monitor stack.
      oop lockee = STACK_OBJECT(-1);
      VERIFY_OOP(lockee);
      // derefing's lockee ought to provoke implicit null check
      // find a free monitor
      BasicObjectLock* entry = (BasicObjectLock*) istate->stack_base();
      assert(entry->obj() == NULL, "Frame manager didn't allocate the monitor");
      entry->set_obj(lockee);
      bool success = false;
      uintptr_t epoch_mask_in_place = (uintptr_t)markOopDesc::epoch_mask_in_place;

      markOop mark = lockee->mark();
      intptr_t hash = (intptr_t) markOopDesc::no_hash;
      // implies UseBiasedLocking
      if (mark->has_bias_pattern()) {
        uintptr_t thread_ident;
        uintptr_t anticipated_bias_locking_value;
        thread_ident = (uintptr_t)istate->thread();
        anticipated_bias_locking_value =
          (((uintptr_t)lockee->klass()->prototype_header() | thread_ident) ^ (uintptr_t)mark) &
          ~((uintptr_t) markOopDesc::age_mask_in_place);

        if  (anticipated_bias_locking_value == 0) {
          // already biased towards this thread, nothing to do
          if (PrintBiasedLockingStatistics) {
            (* BiasedLocking::biased_lock_entry_count_addr())++;
          }
          success = true;
        } else if ((anticipated_bias_locking_value & markOopDesc::biased_lock_mask_in_place) != 0) {
          // try revoke bias
          markOop header = lockee->klass()->prototype_header();
          if (hash != markOopDesc::no_hash) {
            header = header->copy_set_hash(hash);
          }
          if (lockee->cas_set_mark(header, mark) == mark) {
            if (PrintBiasedLockingStatistics) {
              (*BiasedLocking::revoked_lock_entry_count_addr())++;
            }
          }
        } else if ((anticipated_bias_locking_value & epoch_mask_in_place) !=0) {
          // try rebias
          markOop new_header = (markOop) ( (intptr_t) lockee->klass()->prototype_header() | thread_ident);
          if (hash != markOopDesc::no_hash) {
                new_header = new_header->copy_set_hash(hash);
          }
          if (lockee->cas_set_mark(new_header, mark) == mark) {
            if (PrintBiasedLockingStatistics) {
              (* BiasedLocking::rebiased_lock_entry_count_addr())++;
            }
          } else {
            CALL_VM(InterpreterRuntime::monitorenter(THREAD, entry), handle_exception);
          }
          success = true;
        } else {
          // try to bias towards thread in case object is anonymously biased
          markOop header = (markOop) ((uintptr_t) mark & ((uintptr_t)markOopDesc::biased_lock_mask_in_place |
                                                          (uintptr_t)markOopDesc::age_mask_in_place | epoch_mask_in_place));
          if (hash != markOopDesc::no_hash) {
            header = header->copy_set_hash(hash);
          }
          markOop new_header = (markOop) ((uintptr_t) header | thread_ident);
          // debugging hint
          DEBUG_ONLY(entry->lock()->set_displaced_header((markOop) (uintptr_t) 0xdeaddead);)
          if (lockee->cas_set_mark(new_header, header) == header) {
            if (PrintBiasedLockingStatistics) {
              (* BiasedLocking::anonymously_biased_lock_entry_count_addr())++;
            }
          } else {
            CALL_VM(InterpreterRuntime::monitorenter(THREAD, entry), handle_exception);
          }
          success = true;
        }
      }

      // traditional lightweight locking
      if (!success) {
        markOop displaced = lockee->mark()->set_unlocked();
        entry->lock()->set_displaced_header(displaced);
        bool call_vm = UseHeavyMonitors;
        if (call_vm || lockee->cas_set_mark((markOop)entry, displaced) != displaced) {
          // Is it simple recursive case?
          if (!call_vm && THREAD->is_lock_owned((address) displaced->clear_lock_bits())) {
            entry->lock()->set_displaced_header(NULL);
          } else {
            CALL_VM(InterpreterRuntime::monitorenter(THREAD, entry), handle_exception);
          }
        }
      }
      UPDATE_PC_AND_TOS(2, -1);
      goto run;
    }
    default: {
      fatal("Unexpected message from frame manager");
    }
  }

run:

  DO_UPDATE_INSTRUCTION_COUNT(READ_BYTECODE(pc));
  DEBUGGER_SINGLE_STEP_NOTIFY();
#ifdef PREFETCH_OPCCODE
  opcode = READ_BYTECODE(pc);  /* prefetch first opcode */
#endif

#ifndef USELABELS
  while (1)
#endif
  {
#ifndef PREFETCH_OPCCODE
      opcode = READ_BYTECODE(pc);
#endif
      // Seems like this happens twice per opcode. At worst this is only
      // need at entry to the loop.
      // DEBUGGER_SINGLE_STEP_NOTIFY();
      /* Using this labels avoids double breakpoints when quickening and
       * when returing from transition frames.
       */
  opcode_switch:
      assert(istate == orig, "Corrupted istate");
      /* QQQ Hmm this has knowledge of direction, ought to be a stack method */
      assert(topOfStack >= istate->stack_limit(), "Stack overrun");
      assert(topOfStack < istate->stack_base(), "Stack underrun");

#ifdef USELABELS
      DISPATCH(opcode);
#else
//      intptr_t* stackBase = istate->stack_base() - Interpreter::stackElementWords;
//      int stackSize = (int) (stackBase - topOfStack);
//      printf("bci: %i, bytecode: %x, stack: %i, [",  BCI(), opcode, stackSize);
//      for (int i = 0; i < stackSize; i++) {
//        printf("(%i,%li,%p)", STACK_INT(-i-1), STACK_LONG(-i-1), STACK_OBJECT(-i-1));
//      }
//      printf("], method: %s, handler: \n", istate->method()->name_and_sig_as_C_string());
      switch (opcode)
#endif
      {
      #include "generated/bytecodeInterpreter.handlers.hpp"
//      #include "bytecodeInterpreter.originalhandlers.hpp"
      DEFAULT:
          fatal("Unimplemented opcode %d = %s", opcode,
                Bytecodes::name((Bytecodes::Code)opcode));
          goto finish;

      } /* switch(opc) */


#ifdef USELABELS
    check_for_exception:
#endif
    {
      if (!THREAD->has_pending_exception()) {
        CONTINUE;
      }
      /* We will be gcsafe soon, so flush our state. */
      DECACHE_PC();
      goto handle_exception;
    }
  do_continue: ;

  } /* while (1) interpreter loop */


  // An exception exists in the thread state see whether this activation can handle it
  handle_exception: {

    HandleMarkCleaner __hmc(THREAD);
    Handle except_oop(THREAD, THREAD->pending_exception());
    // Prevent any subsequent HandleMarkCleaner in the VM
    // from freeing the except_oop handle.
    HandleMark __hm(THREAD);

    THREAD->clear_pending_exception();
    assert(except_oop() != NULL, "No exception to process");
    intptr_t continuation_bci;
    // expression stack is emptied
    topOfStack = istate->stack_base() - Interpreter::stackElementWords;
    CALL_VM(continuation_bci = (intptr_t)InterpreterRuntime::exception_handler_for_exception(THREAD, except_oop()),
            handle_exception);

    except_oop = Handle(THREAD, THREAD->vm_result());
    THREAD->set_vm_result(NULL);
    if (continuation_bci >= 0) {
      // Place exception on top of stack
      SET_STACK_OBJECT(except_oop(), 0);
      MORE_STACK(1);
      pc = METHOD->code_base() + continuation_bci;
      if (log_is_enabled(Info, exceptions)) {
        ResourceMark rm(THREAD);
        stringStream tempst;
        tempst.print("interpreter method <%s>\n"
                     " at bci %d, continuing at %d for thread " INTPTR_FORMAT,
                     METHOD->print_value_string(),
                     (int)(istate->bcp() - METHOD->code_base()),
                     (int)continuation_bci, p2i(THREAD));
        Exceptions::log_exception(except_oop, tempst);
      }
      // for AbortVMOnException flag
      Exceptions::debug_check_abort(except_oop);

      // Update profiling data.
      BI_PROFILE_ALIGN_TO_CURRENT_BCI();
      goto run;
    }
    if (log_is_enabled(Info, exceptions)) {
      ResourceMark rm;
      stringStream tempst;
      tempst.print("interpreter method <%s>\n"
             " at bci %d, unwinding for thread " INTPTR_FORMAT,
             METHOD->print_value_string(),
             (int)(istate->bcp() - METHOD->code_base()),
             p2i(THREAD));
      Exceptions::log_exception(except_oop, tempst);
    }
    // for AbortVMOnException flag
    Exceptions::debug_check_abort(except_oop);

    // No handler in this activation, unwind and try again
    THREAD->set_pending_exception(except_oop(), NULL, 0);
    goto handle_return;
  }  // handle_exception:

  // Return from an interpreter invocation with the result of the interpretation
  // on the top of the Java Stack (or a pending exception)

  handle_Pop_Frame: {

    // We don't really do anything special here except we must be aware
    // that we can get here without ever locking the method (if sync).
    // Also we skip the notification of the exit.

    istate->set_msg(popping_frame);
    // Clear pending so while the pop is in process
    // we don't start another one if a call_vm is done.
    THREAD->clr_pop_frame_pending();
    // Let interpreter (only) see the we're in the process of popping a frame
    THREAD->set_pop_frame_in_process();

    goto handle_return;

  } // handle_Pop_Frame

  // ForceEarlyReturn ends a method, and returns to the caller with a return value
  // given by the invoker of the early return.
  handle_Early_Return: {

    istate->set_msg(early_return);

    // Clear expression stack.
    topOfStack = istate->stack_base() - Interpreter::stackElementWords;

    JvmtiThreadState *ts = THREAD->jvmti_thread_state();

    // Push the value to be returned.
    switch (istate->method()->result_type()) {
      case T_BOOLEAN:
      case T_SHORT:
      case T_BYTE:
      case T_CHAR:
      case T_INT:
        SET_STACK_INT(ts->earlyret_value().i, 0);
        MORE_STACK(1);
        break;
      case T_LONG:
        SET_STACK_LONG(ts->earlyret_value().j, 1);
        MORE_STACK(2);
        break;
      case T_FLOAT:
        SET_STACK_FLOAT(ts->earlyret_value().f, 0);
        MORE_STACK(1);
        break;
      case T_DOUBLE:
        SET_STACK_DOUBLE(ts->earlyret_value().d, 1);
        MORE_STACK(2);
        break;
      case T_ARRAY:
      case T_OBJECT:
        SET_STACK_OBJECT(ts->earlyret_oop(), 0);
        MORE_STACK(1);
        break;
    }

    ts->clr_earlyret_value();
    ts->set_earlyret_oop(NULL);
    ts->clr_earlyret_pending();

    // Fall through to handle_return.

  } // handle_Early_Return

  handle_return: {
    // A storestore barrier is required to order initialization of
    // final fields with publishing the reference to the object that
    // holds the field. Without the barrier the value of final fields
    // can be observed to change.
    OrderAccess::storestore();

    DECACHE_STATE();

    bool suppress_error = istate->msg() == popping_frame || istate->msg() == early_return;
    bool suppress_exit_event = THREAD->has_pending_exception() || istate->msg() == popping_frame;
    Handle original_exception(THREAD, THREAD->pending_exception());
    Handle illegal_state_oop(THREAD, NULL);

    // We'd like a HandleMark here to prevent any subsequent HandleMarkCleaner
    // in any following VM entries from freeing our live handles, but illegal_state_oop
    // isn't really allocated yet and so doesn't become live until later and
    // in unpredicatable places. Instead we must protect the places where we enter the
    // VM. It would be much simpler (and safer) if we could allocate a real handle with
    // a NULL oop in it and then overwrite the oop later as needed. This isn't
    // unfortunately isn't possible.

    THREAD->clear_pending_exception();

    //
    // As far as we are concerned we have returned. If we have a pending exception
    // that will be returned as this invocation's result. However if we get any
    // exception(s) while checking monitor state one of those IllegalMonitorStateExceptions
    // will be our final result (i.e. monitor exception trumps a pending exception).
    //

    // If we never locked the method (or really passed the point where we would have),
    // there is no need to unlock it (or look for other monitors), since that
    // could not have happened.

    if (THREAD->do_not_unlock()) {

      // Never locked, reset the flag now because obviously any caller must
      // have passed their point of locking for us to have gotten here.

      THREAD->clr_do_not_unlock();
    } else {
      // At this point we consider that we have returned. We now check that the
      // locks were properly block structured. If we find that they were not
      // used properly we will return with an illegal monitor exception.
      // The exception is checked by the caller not the callee since this
      // checking is considered to be part of the invocation and therefore
      // in the callers scope (JVM spec 8.13).
      //
      // Another weird thing to watch for is if the method was locked
      // recursively and then not exited properly. This means we must
      // examine all the entries in reverse time(and stack) order and
      // unlock as we find them. If we find the method monitor before
      // we are at the initial entry then we should throw an exception.
      // It is not clear the template based interpreter does this
      // correctly

      BasicObjectLock* base = istate->monitor_base();
      BasicObjectLock* end = (BasicObjectLock*) istate->stack_base();
      bool method_unlock_needed = METHOD->is_synchronized();
      // We know the initial monitor was used for the method don't check that
      // slot in the loop
      if (method_unlock_needed) base--;

      // Check all the monitors to see they are unlocked. Install exception if found to be locked.
      while (end < base) {
        oop lockee = end->obj();
        if (lockee != NULL) {
          BasicLock* lock = end->lock();
          markOop header = lock->displaced_header();
          end->set_obj(NULL);

          if (!lockee->mark()->has_bias_pattern()) {
            // If it isn't recursive we either must swap old header or call the runtime
            if (header != NULL) {
              markOop old_header = markOopDesc::encode(lock);
              if (lockee->cas_set_mark(header, old_header) != old_header) {
                // restore object for the slow case
                end->set_obj(lockee);
                {
                  // Prevent any HandleMarkCleaner from freeing our live handles
                  HandleMark __hm(THREAD);
                  CALL_VM_NOCHECK(InterpreterRuntime::monitorexit(THREAD, end));
                }
              }
            }
          }
          // One error is plenty
          if (illegal_state_oop() == NULL && !suppress_error) {
            {
              // Prevent any HandleMarkCleaner from freeing our live handles
              HandleMark __hm(THREAD);
              CALL_VM_NOCHECK(InterpreterRuntime::throw_illegal_monitor_state_exception(THREAD));
            }
            assert(THREAD->has_pending_exception(), "Lost our exception!");
            illegal_state_oop = Handle(THREAD, THREAD->pending_exception());
            THREAD->clear_pending_exception();
          }
        }
        end++;
      }
      // Unlock the method if needed
      if (method_unlock_needed) {
        if (base->obj() == NULL) {
          // The method is already unlocked this is not good.
          if (illegal_state_oop() == NULL && !suppress_error) {
            {
              // Prevent any HandleMarkCleaner from freeing our live handles
              HandleMark __hm(THREAD);
              CALL_VM_NOCHECK(InterpreterRuntime::throw_illegal_monitor_state_exception(THREAD));
            }
            assert(THREAD->has_pending_exception(), "Lost our exception!");
            illegal_state_oop = Handle(THREAD, THREAD->pending_exception());
            THREAD->clear_pending_exception();
          }
        } else {
          //
          // The initial monitor is always used for the method
          // However if that slot is no longer the oop for the method it was unlocked
          // and reused by something that wasn't unlocked!
          //
          // deopt can come in with rcvr dead because c2 knows
          // its value is preserved in the monitor. So we can't use locals[0] at all
          // and must use first monitor slot.
          //
          oop rcvr = base->obj();
          if (rcvr == NULL) {
            if (!suppress_error) {
              VM_JAVA_ERROR_NO_JUMP(vmSymbols::java_lang_NullPointerException(), "", note_nullCheck_trap);
              illegal_state_oop = Handle(THREAD, THREAD->pending_exception());
              THREAD->clear_pending_exception();
            }
          } else if (UseHeavyMonitors) {
            {
              // Prevent any HandleMarkCleaner from freeing our live handles.
              HandleMark __hm(THREAD);
              CALL_VM_NOCHECK(InterpreterRuntime::monitorexit(THREAD, base));
            }
            if (THREAD->has_pending_exception()) {
              if (!suppress_error) illegal_state_oop = Handle(THREAD, THREAD->pending_exception());
              THREAD->clear_pending_exception();
            }
          } else {
            BasicLock* lock = base->lock();
            markOop header = lock->displaced_header();
            base->set_obj(NULL);

            if (!rcvr->mark()->has_bias_pattern()) {
              base->set_obj(NULL);
              // If it isn't recursive we either must swap old header or call the runtime
              if (header != NULL) {
                markOop old_header = markOopDesc::encode(lock);
                if (rcvr->cas_set_mark(header, old_header) != old_header) {
                  // restore object for the slow case
                  base->set_obj(rcvr);
                  {
                    // Prevent any HandleMarkCleaner from freeing our live handles
                    HandleMark __hm(THREAD);
                    CALL_VM_NOCHECK(InterpreterRuntime::monitorexit(THREAD, base));
                  }
                  if (THREAD->has_pending_exception()) {
                    if (!suppress_error) illegal_state_oop = Handle(THREAD, THREAD->pending_exception());
                    THREAD->clear_pending_exception();
                  }
                }
              }
            }
          }
        }
      }
    }
    // Clear the do_not_unlock flag now.
    THREAD->clr_do_not_unlock();

    //
    // Notify jvmti/jvmdi
    //
    // NOTE: we do not notify a method_exit if we have a pending exception,
    // including an exception we generate for unlocking checks.  In the former
    // case, JVMDI has already been notified by our call for the exception handler
    // and in both cases as far as JVMDI is concerned we have already returned.
    // If we notify it again JVMDI will be all confused about how many frames
    // are still on the stack (4340444).
    //
    // NOTE Further! It turns out the the JVMTI spec in fact expects to see
    // method_exit events whenever we leave an activation unless it was done
    // for popframe. This is nothing like jvmdi. However we are passing the
    // tests at the moment (apparently because they are jvmdi based) so rather
    // than change this code and possibly fail tests we will leave it alone
    // (with this note) in anticipation of changing the vm and the tests
    // simultaneously.


    //
    suppress_exit_event = suppress_exit_event || illegal_state_oop() != NULL;



#ifdef VM_JVMTI
      if (_jvmti_interp_events) {
        // Whenever JVMTI puts a thread in interp_only_mode, method
        // entry/exit events are sent for that thread to track stack depth.
        if ( !suppress_exit_event && THREAD->is_interp_only_mode() ) {
          {
            // Prevent any HandleMarkCleaner from freeing our live handles
            HandleMark __hm(THREAD);
            CALL_VM_NOCHECK(InterpreterRuntime::post_method_exit(THREAD));
          }
        }
      }
#endif /* VM_JVMTI */

    //
    // See if we are returning any exception
    // A pending exception that was pending prior to a possible popping frame
    // overrides the popping frame.
    //
    assert(!suppress_error || (suppress_error && illegal_state_oop() == NULL), "Error was not suppressed");
    if (illegal_state_oop() != NULL || original_exception() != NULL) {
      // Inform the frame manager we have no result.
      istate->set_msg(throwing_exception);
      if (illegal_state_oop() != NULL)
        THREAD->set_pending_exception(illegal_state_oop(), NULL, 0);
      else
        THREAD->set_pending_exception(original_exception(), NULL, 0);
      UPDATE_PC_AND_RETURN(0);
    }

    if (istate->msg() == popping_frame) {
      // Make it simpler on the assembly code and set the message for the frame pop.
      // returns
      if (istate->prev() == NULL) {
        // We must be returning to a deoptimized frame (because popframe only happens between
        // two interpreted frames). We need to save the current arguments in C heap so that
        // the deoptimized frame when it restarts can copy the arguments to its expression
        // stack and re-execute the call. We also have to notify deoptimization that this
        // has occurred and to pick the preserved args copy them to the deoptimized frame's
        // java expression stack. Yuck.
        //
        THREAD->popframe_preserve_args(in_ByteSize(METHOD->size_of_parameters() * wordSize),
                                LOCALS_SLOT(METHOD->size_of_parameters() - 1));
        THREAD->set_popframe_condition_bit(JavaThread::popframe_force_deopt_reexecution_bit);
      }
    } else {
      istate->set_msg(return_from_method);
    }

    // Normal return
    // Advance the pc and return to frame manager
    UPDATE_PC_AND_RETURN(2);
  } /* handle_return: */

// This is really a fatal error return

finish:
  DECACHE_TOS();
  DECACHE_PC();

  return;
}

/*
 * All the code following this point is only produced once and is not present
 * in the JVMTI version of the interpreter
*/

#ifndef VM_JVMTI

// This constructor should only be used to contruct the object to signal
// interpreter initialization. All other instances should be created by
// the frame manager.
BytecodeInterpreter::BytecodeInterpreter(messages msg) {
  if (msg != initialize) ShouldNotReachHere();
  _msg = msg;
  _self_link = this;
  _prev_link = NULL;
}

// Inline static functions for Java Stack and Local manipulation

// The implementations are platform dependent. We have to worry about alignment
// issues on some machines which can change on the same platform depending on
// whether it is an LP64 machine also.
address BytecodeInterpreter::stack_slot(intptr_t *tos, int offset) {
  return (address) tos[Interpreter::expr_index_at(-offset)];
}

jint BytecodeInterpreter::stack_int(intptr_t *tos, int offset) {
  return *((jint*) &tos[Interpreter::expr_index_at(-offset)]);
}

jfloat BytecodeInterpreter::stack_float(intptr_t *tos, int offset) {
  return *((jfloat *) &tos[Interpreter::expr_index_at(-offset)]);
}

oop BytecodeInterpreter::stack_object(intptr_t *tos, int offset) {
  return cast_to_oop(tos [Interpreter::expr_index_at(-offset)]);
}

jdouble BytecodeInterpreter::stack_double(intptr_t *tos, int offset) {
  return ((VMJavaVal64*) &tos[Interpreter::expr_index_at(-offset)])->d;
}

jlong BytecodeInterpreter::stack_long(intptr_t *tos, int offset) {
  return ((VMJavaVal64 *) &tos[Interpreter::expr_index_at(-offset)])->l;
}

// only used for value types
void BytecodeInterpreter::set_stack_slot(intptr_t *tos, address value,
                                                        int offset) {
  *((address *)&tos[Interpreter::expr_index_at(-offset)]) = value;
}

void BytecodeInterpreter::set_stack_int(intptr_t *tos, int value,
                                                       int offset) {
  *((jint *)&tos[Interpreter::expr_index_at(-offset)]) = value;
}

void BytecodeInterpreter::set_stack_float(intptr_t *tos, jfloat value,
                                                         int offset) {
  *((jfloat *)&tos[Interpreter::expr_index_at(-offset)]) = value;
}

void BytecodeInterpreter::set_stack_object(intptr_t *tos, oop value,
                                                          int offset) {
  *((oop *)&tos[Interpreter::expr_index_at(-offset)]) = value;
}

// needs to be platform dep for the 32 bit platforms.
void BytecodeInterpreter::set_stack_double(intptr_t *tos, jdouble value,
                                                          int offset) {
  ((VMJavaVal64*)&tos[Interpreter::expr_index_at(-offset)])->d = value;
}

void BytecodeInterpreter::set_stack_double_from_addr(intptr_t *tos,
                                              address addr, int offset) {
  (((VMJavaVal64*)&tos[Interpreter::expr_index_at(-offset)])->d =
                        ((VMJavaVal64*)addr)->d);
}

void BytecodeInterpreter::set_stack_long(intptr_t *tos, jlong value,
                                                        int offset) {
  ((VMJavaVal64*)&tos[Interpreter::expr_index_at(-offset+1)])->l = 0xdeedbeeb;
  ((VMJavaVal64*)&tos[Interpreter::expr_index_at(-offset)])->l = value;
}

void BytecodeInterpreter::set_stack_long_from_addr(intptr_t *tos,
                                            address addr, int offset) {
  ((VMJavaVal64*)&tos[Interpreter::expr_index_at(-offset+1)])->l = 0xdeedbeeb;
  ((VMJavaVal64*)&tos[Interpreter::expr_index_at(-offset)])->l =
                        ((VMJavaVal64*)addr)->l;
}

// Locals

address BytecodeInterpreter::locals_slot(intptr_t* locals, int offset) {
  return (address)locals[Interpreter::local_index_at(-offset)];
}
jint BytecodeInterpreter::locals_int(intptr_t* locals, int offset) {
  return (jint)locals[Interpreter::local_index_at(-offset)];
}
jfloat BytecodeInterpreter::locals_float(intptr_t* locals, int offset) {
  return (jfloat)locals[Interpreter::local_index_at(-offset)];
}
oop BytecodeInterpreter::locals_object(intptr_t* locals, int offset) {
  return cast_to_oop(locals[Interpreter::local_index_at(-offset)]);
}
jdouble BytecodeInterpreter::locals_double(intptr_t* locals, int offset) {
  return ((VMJavaVal64*)&locals[Interpreter::local_index_at(-(offset+1))])->d;
}
jlong BytecodeInterpreter::locals_long(intptr_t* locals, int offset) {
  return ((VMJavaVal64*)&locals[Interpreter::local_index_at(-(offset+1))])->l;
}

// Returns the address of locals value.
address BytecodeInterpreter::locals_long_at(intptr_t* locals, int offset) {
  return ((address)&locals[Interpreter::local_index_at(-(offset+1))]);
}
address BytecodeInterpreter::locals_double_at(intptr_t* locals, int offset) {
  return ((address)&locals[Interpreter::local_index_at(-(offset+1))]);
}

// Used for local value or returnAddress
void BytecodeInterpreter::set_locals_slot(intptr_t *locals,
                                   address value, int offset) {
  *((address*)&locals[Interpreter::local_index_at(-offset)]) = value;
}
void BytecodeInterpreter::set_locals_int(intptr_t *locals,
                                   jint value, int offset) {
  *((jint *)&locals[Interpreter::local_index_at(-offset)]) = value;
}
void BytecodeInterpreter::set_locals_float(intptr_t *locals,
                                   jfloat value, int offset) {
  *((jfloat *)&locals[Interpreter::local_index_at(-offset)]) = value;
}
void BytecodeInterpreter::set_locals_object(intptr_t *locals,
                                   oop value, int offset) {
  *((oop *)&locals[Interpreter::local_index_at(-offset)]) = value;
}
void BytecodeInterpreter::set_locals_double(intptr_t *locals,
                                   jdouble value, int offset) {
  ((VMJavaVal64*)&locals[Interpreter::local_index_at(-(offset+1))])->d = value;
}
void BytecodeInterpreter::set_locals_long(intptr_t *locals,
                                   jlong value, int offset) {
  ((VMJavaVal64*)&locals[Interpreter::local_index_at(-(offset+1))])->l = value;
}
void BytecodeInterpreter::set_locals_double_from_addr(intptr_t *locals,
                                   address addr, int offset) {
  ((VMJavaVal64*)&locals[Interpreter::local_index_at(-(offset+1))])->d = ((VMJavaVal64*)addr)->d;
}
void BytecodeInterpreter::set_locals_long_from_addr(intptr_t *locals,
                                   address addr, int offset) {
  ((VMJavaVal64*)&locals[Interpreter::local_index_at(-(offset+1))])->l = ((VMJavaVal64*)addr)->l;
}

void BytecodeInterpreter::astore(intptr_t* tos,    int stack_offset,
                          intptr_t* locals, int locals_offset) {
  intptr_t value = tos[Interpreter::expr_index_at(-stack_offset)];
  locals[Interpreter::local_index_at(-locals_offset)] = value;
}


void BytecodeInterpreter::copy_stack_slot(intptr_t *tos, int from_offset,
                                   int to_offset) {
  tos[Interpreter::expr_index_at(-to_offset)] =
                      (intptr_t)tos[Interpreter::expr_index_at(-from_offset)];
}

void BytecodeInterpreter::dup(intptr_t *tos) {
  copy_stack_slot(tos, -1, 0);
}
void BytecodeInterpreter::dup2(intptr_t *tos) {
  copy_stack_slot(tos, -2, 0);
  copy_stack_slot(tos, -1, 1);
}

void BytecodeInterpreter::dup_x1(intptr_t *tos) {
  /* insert top word two down */
  copy_stack_slot(tos, -1, 0);
  copy_stack_slot(tos, -2, -1);
  copy_stack_slot(tos, 0, -2);
}

void BytecodeInterpreter::dup_x2(intptr_t *tos) {
  /* insert top word three down  */
  copy_stack_slot(tos, -1, 0);
  copy_stack_slot(tos, -2, -1);
  copy_stack_slot(tos, -3, -2);
  copy_stack_slot(tos, 0, -3);
}
void BytecodeInterpreter::dup2_x1(intptr_t *tos) {
  /* insert top 2 slots three down */
  copy_stack_slot(tos, -1, 1);
  copy_stack_slot(tos, -2, 0);
  copy_stack_slot(tos, -3, -1);
  copy_stack_slot(tos, 1, -2);
  copy_stack_slot(tos, 0, -3);
}
void BytecodeInterpreter::dup2_x2(intptr_t *tos) {
  /* insert top 2 slots four down */
  copy_stack_slot(tos, -1, 1);
  copy_stack_slot(tos, -2, 0);
  copy_stack_slot(tos, -3, -1);
  copy_stack_slot(tos, -4, -2);
  copy_stack_slot(tos, 1, -3);
  copy_stack_slot(tos, 0, -4);
}


void BytecodeInterpreter::swap(intptr_t *tos) {
  // swap top two elements
  intptr_t val = tos[Interpreter::expr_index_at(1)];
  // Copy -2 entry to -1
  copy_stack_slot(tos, -2, -1);
  // Store saved -1 entry into -2
  tos[Interpreter::expr_index_at(2)] = val;
}
// --------------------------------------------------------------------------------
// Non-product code
#ifndef PRODUCT

const char* BytecodeInterpreter::C_msg(BytecodeInterpreter::messages msg) {
  switch (msg) {
     case BytecodeInterpreter::no_request:  return("no_request");
     case BytecodeInterpreter::initialize:  return("initialize");
     // status message to C++ interpreter
     case BytecodeInterpreter::method_entry:  return("method_entry");
     case BytecodeInterpreter::method_resume:  return("method_resume");
     case BytecodeInterpreter::got_monitors:  return("got_monitors");
     case BytecodeInterpreter::rethrow_exception:  return("rethrow_exception");
     // requests to frame manager from C++ interpreter
     case BytecodeInterpreter::call_method:  return("call_method");
     case BytecodeInterpreter::return_from_method:  return("return_from_method");
     case BytecodeInterpreter::more_monitors:  return("more_monitors");
     case BytecodeInterpreter::throwing_exception:  return("throwing_exception");
     case BytecodeInterpreter::popping_frame:  return("popping_frame");
     case BytecodeInterpreter::do_osr:  return("do_osr");
     // deopt
     case BytecodeInterpreter::deopt_resume:  return("deopt_resume");
     case BytecodeInterpreter::deopt_resume2:  return("deopt_resume2");
     default: return("BAD MSG");
  }
}
void
BytecodeInterpreter::print() {
  tty->print_cr("thread: " INTPTR_FORMAT, (uintptr_t) this->_thread);
  tty->print_cr("bcp: " INTPTR_FORMAT, (uintptr_t) this->_bcp);
  tty->print_cr("locals: " INTPTR_FORMAT, (uintptr_t) this->_locals);
  tty->print_cr("constants: " INTPTR_FORMAT, (uintptr_t) this->_constants);
  {
    ResourceMark rm;
    char *method_name = _method->name_and_sig_as_C_string();
    tty->print_cr("method: " INTPTR_FORMAT "[ %s ]",  (uintptr_t) this->_method, method_name);
  }
  tty->print_cr("mdx: " INTPTR_FORMAT, (uintptr_t) this->_mdx);
  tty->print_cr("stack: " INTPTR_FORMAT, (uintptr_t) this->_stack);
  tty->print_cr("msg: %s", C_msg(this->_msg));
  tty->print_cr("result_to_call._callee: " INTPTR_FORMAT, (uintptr_t) this->_result._to_call._callee);
  tty->print_cr("result_to_call._callee_entry_point: " INTPTR_FORMAT, (uintptr_t) this->_result._to_call._callee_entry_point);
  tty->print_cr("result_to_call._bcp_advance: %d ", this->_result._to_call._bcp_advance);
  tty->print_cr("osr._osr_buf: " INTPTR_FORMAT, (uintptr_t) this->_result._osr._osr_buf);
  tty->print_cr("osr._osr_entry: " INTPTR_FORMAT, (uintptr_t) this->_result._osr._osr_entry);
  tty->print_cr("prev_link: " INTPTR_FORMAT, (uintptr_t) this->_prev_link);
  tty->print_cr("native_mirror: " INTPTR_FORMAT, (uintptr_t) p2i(this->_oop_temp));
  tty->print_cr("stack_base: " INTPTR_FORMAT, (uintptr_t) this->_stack_base);
  tty->print_cr("stack_limit: " INTPTR_FORMAT, (uintptr_t) this->_stack_limit);
  tty->print_cr("monitor_base: " INTPTR_FORMAT, (uintptr_t) this->_monitor_base);
#ifdef SPARC
  tty->print_cr("last_Java_pc: " INTPTR_FORMAT, (uintptr_t) this->_last_Java_pc);
  tty->print_cr("frame_bottom: " INTPTR_FORMAT, (uintptr_t) this->_frame_bottom);
  tty->print_cr("&native_fresult: " INTPTR_FORMAT, (uintptr_t) &this->_native_fresult);
  tty->print_cr("native_lresult: " INTPTR_FORMAT, (uintptr_t) this->_native_lresult);
#endif
#if !defined(ZERO) && defined(PPC)
  tty->print_cr("last_Java_fp: " INTPTR_FORMAT, (uintptr_t) this->_last_Java_fp);
#endif // !ZERO
  tty->print_cr("self_link: " INTPTR_FORMAT, (uintptr_t) this->_self_link);
}

extern "C" {
  void PI(uintptr_t arg) {
    ((BytecodeInterpreter*)arg)->print();
  }
}
#endif // PRODUCT

#endif // JVMTI
#endif // CC_INTERP
