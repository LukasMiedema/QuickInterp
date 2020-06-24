/*
 * Bunch of macros for all the instructions.
 * A superinstruction set generator can use these macros
 * to more easily build an instruction set, leaving the
 * actual concatenation to the C++ preprocessor
 *
 * The macros do not increment the PC, instead this is left
 * as separately generated statement at the end of the macro
 * sequence.
 *
 * All signatures are of the form
 * INSTR_ ## instr ## (pc expr for this instruction, stack offset expr for this instruction)
 */

#ifdef USELABELS
#define CASE(opcode) opc ## opcode
#define DEFAULT opc_default
#else
#define CASE(opcode) case Bytecodes:: opcode
#define DEFAULT default
#endif

#define PROFILE_UNCONDITIONAL(pc)                     \
  uint32_t profile_id = Bytes::get_native_u4((pc));   \
  Profiler::visit(profile_id);

#define PROFILE_CONDITIONAL(condition,pc)             \
  if (condition) {                                    \
    PROFILE_UNCONDITIONAL(pc);                        \
  }


// (+0,0) terminal
#define INSTR_end_sequence(pc,offset)                 \
  SET_PC_AND_TOS(pc, offset);                         \
  CONTINUE;

// (+2,+0)
#define INSTR_nop(pc,offset)

// (+2,+1) (all)
#define INSTR_aconst_null(pc,offset)                  \
  SET_STACK_OBJECT(NULL, offset);
#define INSTR_iconst_m1(pc,offset)                    \
  SET_STACK_INT(-1, offset);
#define INSTR_iconst_0(pc,offset)                     \
  SET_STACK_INT(0, offset);
#define INSTR_iconst_1(pc,offset)                     \
  SET_STACK_INT(1, offset);
#define INSTR_iconst_2(pc,offset)                     \
  SET_STACK_INT(2, offset);
#define INSTR_iconst_3(pc,offset)                     \
  SET_STACK_INT(3, offset);
#define INSTR_iconst_4(pc,offset)                     \
  SET_STACK_INT(4, offset);
#define INSTR_iconst_5(pc,offset)                     \
  SET_STACK_INT(5, offset);
#define INSTR_fconst_0(pc,offset)                     \
  SET_STACK_FLOAT(0.0, offset);
#define INSTR_fconst_1(pc,offset)                     \
  SET_STACK_FLOAT(1.0, offset);
#define INSTR_fconst_2(pc,offset)                     \
  SET_STACK_FLOAT(2.0, offset);

// (+2,+2) (all)
#define INSTR_dconst_0(pc,offset)                     \
  SET_STACK_DOUBLE(VMdoubleConstZero(),offset+1);
#define INSTR_dconst_1(pc,offset)                     \
  SET_STACK_DOUBLE(VMdoubleConstOne(),offset+1);
#define INSTR_lconst_0(pc,offset)                     \
  SET_STACK_LONG(VMlongConstZero(),offset+1);
#define INSTR_lconst_1(pc,offset)                     \
  SET_STACK_LONG(VMlongConstOne(),offset+1);

// (+3,+1)
#define INSTR_bipush(pc,offset)                       \
  SET_STACK_INT((jbyte)((pc)[2]),offset);

// (+4,+1)
#define INSTR_sipush(pc,offset)                       \
  SET_STACK_INT((int16_t)Bytes::get_Java_u2((pc) + 2),offset);

// (+3,+1) (all)
#define INSTR_aload(pc,offset)                        \
  VERIFY_OOP(LOCALS_OBJECT((pc)[2]));                 \
  SET_STACK_OBJECT(LOCALS_OBJECT((pc)[2]), offset);
#define INSTR_iload(pc,offset)                        \
  SET_STACK_SLOT(LOCALS_SLOT((pc)[2]), offset);
#define INSTR_fload(pc,offset)                        \
  SET_STACK_SLOT(LOCALS_SLOT((pc)[2]), offset);

// (+3,+2) (all)
#define INSTR_lload(pc,offset)                        \
  SET_STACK_LONG_FROM_ADDR(LOCALS_LONG_AT((pc)[2]), offset+1);
#define INSTR_dload(pc,offset)                        \
  SET_STACK_DOUBLE_FROM_ADDR(LOCALS_DOUBLE_AT((pc)[2]), offset+1);

// (+2,+1) (all)
#define INSTR_aload_0(pc,offset)                      \
  VERIFY_OOP(LOCALS_OBJECT(0));                       \
  SET_STACK_OBJECT(LOCALS_OBJECT(0), offset);
#define INSTR_iload_0(pc,offset)                      \
  SET_STACK_SLOT(LOCALS_SLOT(0), offset);
#define INSTR_fload_0(pc,offset)                      \
  SET_STACK_SLOT(LOCALS_SLOT(0), offset);
#define INSTR_aload_1(pc,offset)                      \
  VERIFY_OOP(LOCALS_OBJECT(1));                       \
  SET_STACK_OBJECT(LOCALS_OBJECT(1), offset);
#define INSTR_iload_1(pc,offset)                      \
  SET_STACK_SLOT(LOCALS_SLOT(1), offset);
#define INSTR_fload_1(pc,offset)                      \
  SET_STACK_SLOT(LOCALS_SLOT(1), offset);
#define INSTR_aload_2(pc,offset)                      \
  VERIFY_OOP(LOCALS_OBJECT(2));                       \
  SET_STACK_OBJECT(LOCALS_OBJECT(2), offset);
#define INSTR_iload_2(pc,offset)                      \
  SET_STACK_SLOT(LOCALS_SLOT(2), offset);
#define INSTR_fload_2(pc,offset)                      \
  SET_STACK_SLOT(LOCALS_SLOT(2), offset);
#define INSTR_aload_3(pc,offset)                      \
  VERIFY_OOP(LOCALS_OBJECT(3));                       \
  SET_STACK_OBJECT(LOCALS_OBJECT(3), offset);
#define INSTR_iload_3(pc,offset)                      \
  SET_STACK_SLOT(LOCALS_SLOT(3), offset);
#define INSTR_fload_3(pc,offset)                      \
  SET_STACK_SLOT(LOCALS_SLOT(3), offset);

// (+2,+2) (all)
#define INSTR_lload_0(pc,offset)                      \
  SET_STACK_LONG_FROM_ADDR(LOCALS_LONG_AT(0), offset+1);
#define INSTR_dload_0(pc,offset)                      \
  SET_STACK_DOUBLE_FROM_ADDR(LOCALS_DOUBLE_AT(0), offset+1);
// (+2,+2) (all)
#define INSTR_lload_1(pc,offset)                      \
  SET_STACK_LONG_FROM_ADDR(LOCALS_LONG_AT(1), offset+1);
#define INSTR_dload_1(pc,offset)                      \
  SET_STACK_DOUBLE_FROM_ADDR(LOCALS_DOUBLE_AT(1), offset+1);
// (+2,+2) (all)
#define INSTR_lload_2(pc,offset)                      \
  SET_STACK_LONG_FROM_ADDR(LOCALS_LONG_AT(2), offset+1);
#define INSTR_dload_2(pc,offset)                      \
  SET_STACK_DOUBLE_FROM_ADDR(LOCALS_DOUBLE_AT(2), offset+1);
// (+2,+2) (all)
#define INSTR_lload_3(pc,offset)                      \
  SET_STACK_LONG_FROM_ADDR(LOCALS_LONG_AT(3), offset+1);
#define INSTR_dload_3(pc,offset)                      \
  SET_STACK_DOUBLE_FROM_ADDR(LOCALS_DOUBLE_AT(3), offset+1);

// (+3,-1) (all)
#define INSTR_astore(pc,offset)                       \
  astore(topOfStack, offset-1, locals, (pc)[2]);
#define INSTR_istore(pc,offset)                       \
  SET_LOCALS_SLOT(STACK_SLOT(-1 + offset), (pc)[2]);
#define INSTR_fstore(pc,offset)                       \
  SET_LOCALS_SLOT(STACK_SLOT(-1 + offset), (pc)[2]);

// (+3,-2) (all)
#define INSTR_lstore(pc,offset)                       \
  SET_LOCALS_LONG(STACK_LONG(-1 + offset), (pc)[2]);
#define INSTR_dstore(pc,offset)                       \
  SET_LOCALS_DOUBLE(STACK_DOUBLE(-1 + offset), (pc)[2]);

// (+?,?) updates-pc updates-tos
#define INSTR_wide(pc,offset)                         \
  uint16_t reg = Bytes::get_Java_u2((pc) + 4);        \
  opcode = READ_BYTECODE((pc) + 2);                   \
  DO_UPDATE_INSTRUCTION_COUNT(opcode);                \
                                                      \
  switch(opcode) {                                    \
      case Bytecodes::_aload:                         \
          VERIFY_OOP(LOCALS_OBJECT(reg));             \
          SET_STACK_OBJECT(LOCALS_OBJECT(reg), offset);\
          SET_PC_AND_TOS(pc + 6, offset + 1);         \
          break;                                      \
                                                      \
      case Bytecodes::_iload:                         \
      case Bytecodes::_fload:                         \
          SET_STACK_SLOT(LOCALS_SLOT(reg), offset);   \
          SET_PC_AND_TOS(pc + 6, offset + 1);         \
          break;                                      \
                                                      \
      case Bytecodes::_lload:                         \
          SET_STACK_LONG_FROM_ADDR(LOCALS_LONG_AT(reg), offset + 1);\
          SET_PC_AND_TOS(pc + 6, offset + 2);         \
          break;                                      \
                                                      \
      case Bytecodes::_dload:                         \
          SET_STACK_DOUBLE_FROM_ADDR(LOCALS_LONG_AT(reg), offset + 1);\
          SET_PC_AND_TOS(pc + 6, offset + 2);         \
          break;                                      \
                                                      \
      case Bytecodes::_astore:                        \
          astore(topOfStack, offset-1, locals, reg);  \
          SET_PC_AND_TOS(pc + 6, offset - 1);         \
          break;                                      \
                                                      \
      case Bytecodes::_istore:                        \
      case Bytecodes::_fstore:                        \
          SET_LOCALS_SLOT(STACK_SLOT(offset-1), reg); \
          SET_PC_AND_TOS(pc + 6, offset - 1);         \
          break;                                      \
                                                      \
      case Bytecodes::_lstore:                        \
          SET_LOCALS_LONG(STACK_LONG(offset-1), reg); \
          SET_PC_AND_TOS(pc + 6, offset - 2);         \
          break;                                      \
                                                      \
      case Bytecodes::_dstore:                        \
          SET_LOCALS_DOUBLE(STACK_DOUBLE(offset-1), reg);\
          SET_PC_AND_TOS(pc + 6, offset - 2);         \
          break;                                      \
                                                      \
      case Bytecodes::_iinc: {                        \
          int16_t incr_offset = (int16_t)Bytes::get_Java_u2((pc)+6);\
          SET_LOCALS_INT(LOCALS_INT(reg) + incr_offset, reg);\
          SET_PC_AND_TOS(pc + 8, offset);             \
          break;                                      \
      }                                               \
      case Bytecodes::_ret:                           \
          BI_PROFILE_UPDATE_RET(/*bci=*/((int)(intptr_t)(LOCALS_ADDR(reg))));\
          SET_PC_AND_TOS(istate->method()->code_base() + (intptr_t)(LOCALS_ADDR(reg)), offset);\
          break;                                      \
      default:                                        \
          VM_JAVA_ERROR(vmSymbols::java_lang_InternalError(), "undefined opcode", note_no_trap);\
  }


// (+2,-1) (all)
#define INSTR_astore_0(pc,offset)                     \
  astore(topOfStack, offset-1, locals, 0);
#define INSTR_istore_0(pc,offset)                     \
  SET_LOCALS_SLOT(STACK_SLOT(-1 + offset), 0);
#define INSTR_fstore_0(pc,offset)                     \
  SET_LOCALS_SLOT(STACK_SLOT(-1 + offset), 0);
#define INSTR_astore_1(pc,offset)                     \
  astore(topOfStack, offset-1, locals, 1);
#define INSTR_istore_1(pc,offset)                     \
  SET_LOCALS_SLOT(STACK_SLOT(-1 + offset), 1);
#define INSTR_fstore_1(pc,offset)                     \
  SET_LOCALS_SLOT(STACK_SLOT(-1 + offset), 1);
#define INSTR_astore_2(pc,offset)                     \
  astore(topOfStack, offset-1, locals, 2);
#define INSTR_istore_2(pc,offset)                     \
  SET_LOCALS_SLOT(STACK_SLOT(-1 + offset), 2);
#define INSTR_fstore_2(pc,offset)                     \
  SET_LOCALS_SLOT(STACK_SLOT(-1 + offset), 2);
#define INSTR_astore_3(pc,offset)                     \
  astore(topOfStack, offset-1, locals, 3);
#define INSTR_istore_3(pc,offset)                     \
  SET_LOCALS_SLOT(STACK_SLOT(-1 + offset), 3);
#define INSTR_fstore_3(pc,offset)                     \
  SET_LOCALS_SLOT(STACK_SLOT(-1 + offset), 3);

// (+2,-2) (all)
#define INSTR_dstore_0(pc,offset)                     \
  SET_LOCALS_DOUBLE(STACK_DOUBLE(-1 + offset), 0);
#define INSTR_lstore_0(pc,offset)                     \
  SET_LOCALS_LONG(STACK_LONG(-1 + offset), 0);
#define INSTR_dstore_1(pc,offset)                     \
  SET_LOCALS_DOUBLE(STACK_DOUBLE(-1 + offset), 1);
#define INSTR_lstore_1(pc,offset)                     \
  SET_LOCALS_LONG(STACK_LONG(-1 + offset), 1);
#define INSTR_dstore_2(pc,offset)                     \
  SET_LOCALS_DOUBLE(STACK_DOUBLE(-1 + offset), 2);
#define INSTR_lstore_2(pc,offset)                     \
  SET_LOCALS_LONG(STACK_LONG(-1 + offset), 2);
#define INSTR_dstore_3(pc,offset)                     \
  SET_LOCALS_DOUBLE(STACK_DOUBLE(-1 + offset), 3);
#define INSTR_lstore_3(pc,offset)                     \
  SET_LOCALS_LONG(STACK_LONG(-1 + offset), 3);

// (+2,-1)
#define INSTR_pop(pc,offset)

// (+2,-2)
#define INSTR_pop2(pc,offset)

// (+2,+1)
#define INSTR_dup(pc,offset)                          \
  dup(topOfStack-offset);

// (+2,+2)
#define INSTR_dup2(pc,offset)                         \
  dup2(topOfStack-offset);

// (+2,+1)
#define INSTR_dup_x1(pc,offset)                       \
  dup_x1(topOfStack-offset);

// (+2,+1)
#define INSTR_dup_x2(pc,offset)                       \
  dup_x2(topOfStack-offset)

// (+2,+2)
#define INSTR_dup2_x1(pc,offset)                      \
  dup2_x1(topOfStack-offset);

// (+2,+2)
#define INSTR_dup2_x2(pc,offset)                      \
  dup2_x2(topOfStack-offset)

// (+2,0)
#define INSTR_swap(pc,offset)                         \
  swap(topOfStack-offset);

// (+2,-1) (all)
#define INSTR_iadd(pc,offset)                         \
  SET_STACK_INT(VMintAdd(STACK_INT(offset-2),         \
                         STACK_INT(offset-1)),        \
                         offset-2);
#define INSTR_isub(pc,offset)                         \
  SET_STACK_INT(VMintSub(STACK_INT(offset-2),         \
                         STACK_INT(offset-1)),        \
                         offset-2);
#define INSTR_imul(pc,offset)                         \
  SET_STACK_INT(VMintMul(STACK_INT(offset-2),         \
                         STACK_INT(offset-1)),        \
                         offset-2);
#define INSTR_iand(pc,offset)                         \
  SET_STACK_INT(VMintAnd(STACK_INT(offset-2),         \
                         STACK_INT(offset-1)),        \
                         offset-2);
#define INSTR_ior(pc,offset)                          \
  SET_STACK_INT(VMintOr(STACK_INT(offset-2),          \
                         STACK_INT(offset-1)),        \
                         offset-2);
#define INSTR_ixor(pc,offset)                         \
  SET_STACK_INT(VMintXor(STACK_INT(offset-2),         \
                         STACK_INT(offset-1)),        \
                         offset-2);
#define INSTR_idiv(pc,offset)                         \
  if (STACK_INT(offset-1) == 0) {                     \
    SET_PC_AND_TOS(pc,offset);                        \
    VM_JAVA_ERROR(vmSymbols::java_lang_ArithmeticException(), \
              "/ by zero", note_div0Check_trap);      \
  }                                                   \
  SET_STACK_INT(VMintDiv(STACK_INT(offset-2),         \
                         STACK_INT(offset-1)),        \
                         offset-2);
#define INSTR_irem(pc,offset)                         \
  if (STACK_INT(offset-1) == 0) {                     \
    SET_PC_AND_TOS(pc,offset);                        \
    VM_JAVA_ERROR(vmSymbols::java_lang_ArithmeticException(), \
              "/ by zero", note_div0Check_trap);      \
  }                                                   \
  SET_STACK_INT(VMintRem(STACK_INT(offset-2),         \
                         STACK_INT(offset-1)),        \
                         offset-2);

// (+2,-2) (all)
#define INSTR_ladd(pc,offset)                         \
  SET_STACK_LONG(VMlongAdd(STACK_LONG(offset-3),      \
                           STACK_LONG(offset-1)),     \
                           offset-3);
#define INSTR_lsub(pc,offset)                         \
  SET_STACK_LONG(VMlongSub(STACK_LONG(offset-3),      \
                           STACK_LONG(offset-1)),     \
                           offset-3);
#define INSTR_lmul(pc,offset)                         \
  SET_STACK_LONG(VMlongMul(STACK_LONG(offset-3),      \
                           STACK_LONG(offset-1)),     \
                           offset-3);
#define INSTR_land(pc,offset)                         \
  SET_STACK_LONG(VMlongAnd(STACK_LONG(offset-3),      \
                           STACK_LONG(offset-1)),     \
                           offset-3);
#define INSTR_lor(pc,offset)                          \
  SET_STACK_LONG(VMlongOr(STACK_LONG(offset-3),       \
                           STACK_LONG(offset-1)),     \
                           offset-3);
#define INSTR_lxor(pc,offset)                         \
  SET_STACK_LONG(VMlongXor(STACK_LONG(offset-3),      \
                           STACK_LONG(offset-1)),     \
                           offset-3);
#define INSTR_ldiv(pc,offset)                         \
  jlong l1 = STACK_LONG(offset-1);                    \
  if (VMlongEqz(l1)) {                                \
    SET_PC_AND_TOS(pc,offset);                        \
    VM_JAVA_ERROR(vmSymbols::java_lang_ArithmeticException(), \
                  "/ by long zero", note_div0Check_trap); \
  }                                                   \
  SET_STACK_LONG(VMlongDiv(STACK_LONG(offset-3),      \
                           l1),                       \
                           offset-3);
#define INSTR_lrem(pc,offset)                         \
  jlong l1 = STACK_LONG(offset-1);                    \
  if (VMlongEqz(l1)) {                                \
    SET_PC_AND_TOS(pc,offset);                        \
    VM_JAVA_ERROR(vmSymbols::java_lang_ArithmeticException(), \
                  "/ by long zero", note_div0Check_trap); \
  }                                                   \
  SET_STACK_LONG(VMlongRem(STACK_LONG(offset-3),      \
                           l1),                       \
                           offset-3);

// (+2,-2) (all)
#define INSTR_dadd(pc,offset)                         \
  SET_STACK_DOUBLE(VMdoubleAdd(STACK_DOUBLE(offset-3),\
                               STACK_DOUBLE(offset-1)),\
                               offset-3);
#define INSTR_dsub(pc,offset)                         \
  SET_STACK_DOUBLE(VMdoubleSub(STACK_DOUBLE(offset-3),\
                               STACK_DOUBLE(offset-1)),\
                               offset-3);
#define INSTR_dmul(pc,offset)                         \
  SET_STACK_DOUBLE(VMdoubleMul(STACK_DOUBLE(offset-3),\
                               STACK_DOUBLE(offset-1)),\
                               offset-3);
#define INSTR_ddiv(pc,offset)                         \
  SET_STACK_DOUBLE(VMdoubleDiv(STACK_DOUBLE(offset-3),\
                               STACK_DOUBLE(offset-1)),\
                               offset-3);
#define INSTR_drem(pc,offset)                         \
  SET_STACK_DOUBLE(VMdoubleRem(STACK_DOUBLE(offset-3),\
                               STACK_DOUBLE(offset-1)),\
                               offset-3);

// (+2,-1) (all)
#define INSTR_fadd(pc,offset)                         \
  SET_STACK_FLOAT(VMfloatAdd(STACK_FLOAT(offset-2),   \
                               STACK_FLOAT(offset-1)),\
                               offset-2);
#define INSTR_fsub(pc,offset)                         \
  SET_STACK_FLOAT(VMfloatSub(STACK_FLOAT(offset-2),   \
                               STACK_FLOAT(offset-1)),\
                               offset-2);
#define INSTR_fmul(pc,offset)                         \
  SET_STACK_FLOAT(VMfloatMul(STACK_FLOAT(offset-2),   \
                               STACK_FLOAT(offset-1)),\
                               offset-2);
#define INSTR_fdiv(pc,offset)                         \
  SET_STACK_FLOAT(VMfloatDiv(STACK_FLOAT(offset-2),   \
                               STACK_FLOAT(offset-1)),\
                               offset-2);
#define INSTR_frem(pc,offset)                         \
  SET_STACK_FLOAT(VMfloatRem(STACK_FLOAT(offset-2) ,  \
                               STACK_FLOAT(offset-1)),\
                               offset-2);

// (+2,-1) (all)
#define INSTR_ishl(pc,offset)                         \
  SET_STACK_INT(VMintShl(STACK_INT(offset-2),         \
                         STACK_INT(offset-1)),        \
                         offset-2);
#define INSTR_ishr(pc,offset)                         \
  SET_STACK_INT(VMintShr(STACK_INT(offset-2),         \
                         STACK_INT(offset-1)),        \
                         offset-2);
#define INSTR_iushr(pc,offset)                        \
  SET_STACK_INT(VMintUshr(STACK_INT(offset-2),        \
                         STACK_INT(offset-1)),        \
                         offset-2);
#define INSTR_lshl(pc,offset)                         \
  SET_STACK_LONG(VMlongShl(STACK_LONG(offset-2),      \
                         STACK_INT(offset-1)),        \
                         offset-2);
#define INSTR_lshr(pc,offset)                         \
  SET_STACK_LONG(VMlongShr(STACK_LONG(offset-2),      \
                         STACK_INT(offset-1)),        \
                         offset-2);
#define INSTR_lushr(pc,offset)                        \
  SET_STACK_LONG(VMlongUshr(STACK_LONG(offset-2),     \
                         STACK_INT(offset-1)),        \
                         offset-2);

// (+4,0)
#define INSTR_iinc(pc,offset)                         \
  SET_LOCALS_INT(LOCALS_INT((pc)[2]) + (jbyte)((pc)[3]), (pc)[2]);

// (+2,0) (all)
#define INSTR_ineg(pc,offset)                         \
  SET_STACK_INT(VMintNeg(STACK_INT(offset-1)), offset-1);
#define INSTR_fneg(pc,offset)                         \
  SET_STACK_FLOAT(VMfloatNeg(STACK_FLOAT(offset-1)), offset-1);
#define INSTR_lneg(pc,offset)                         \
  SET_STACK_LONG(VMlongNeg(STACK_LONG(offset-1)), offset-1);
#define INSTR_dneg(pc,offset)                         \
  SET_STACK_DOUBLE(VMdoubleNeg(STACK_DOUBLE(offset-1)), offset-1);

// (+2,0)
#define INSTR_i2f(pc,offset)                          \
  SET_STACK_FLOAT(VMint2Float(STACK_INT(offset-1)), offset-1);

// NOTE: original code was very ugly for i2l, i2d. l2i, l2f, l2d, f2l, f2d, d2i, d2f, d2l
// (+2,+1)
#define INSTR_i2l(pc,offset)                          \
  SET_STACK_LONG(VMint2Long(STACK_INT(offset-1)), offset);

// (+2,+1)
#define INSTR_i2d(pc,offset)                          \
  SET_STACK_DOUBLE((jdouble) ((jlong)STACK_INT(offset-1)), offset);

// (+2,-1)
#define INSTR_l2i(pc,offset)                          \
  SET_STACK_INT(VMlong2Int(STACK_LONG(offset-1)), offset-2);

// (+2,-1)
#define INSTR_l2f(pc,offset)                          \
  SET_STACK_FLOAT(VMlong2Float(STACK_LONG(offset-1)), offset-2);

// (+2,0)
#define INSTR_l2d(pc,offset)                          \
  SET_STACK_DOUBLE(VMlong2Double(STACK_LONG(offset-1)), offset-1);

// (+2,0)
#define INSTR_f2i(pc,offset)                          \
  SET_STACK_INT(SharedRuntime::f2i(STACK_FLOAT(offset-1)), offset-1);

// (+2,+1)
#define INSTR_f2l(pc,offset)                          \
  SET_STACK_LONG(SharedRuntime::f2l(STACK_FLOAT(offset-1)), offset);

// (+2,+1)
#define INSTR_f2d(pc,offset)                          \
  SET_STACK_DOUBLE((jdouble) STACK_FLOAT(offset-1), offset);

// (+2,-1)
#define INSTR_d2i(pc,offset)                          \
  SET_STACK_INT(SharedRuntime::d2i(STACK_DOUBLE(offset-1)), offset-2);

// (+2,-1)
#define INSTR_d2f(pc,offset)                          \
  SET_STACK_FLOAT(VMdouble2Float(STACK_DOUBLE(offset-1)), offset-2);

// (+2,0)
#define INSTR_d2l(pc,offset)                          \
  SET_STACK_LONG(SharedRuntime::d2l(STACK_DOUBLE(offset-1)), offset-1);

// (+2,0) (all)
#define INSTR_i2b(pc,offset)                          \
  SET_STACK_INT(VMint2Byte(STACK_INT(offset-1)), offset-1);
#define INSTR_i2c(pc,offset)                          \
  SET_STACK_INT(VMint2Char(STACK_INT(offset-1)), offset-1);
#define INSTR_i2s(pc,offset)                          \
  SET_STACK_INT(VMint2Short(STACK_INT(offset-1)), offset-1);

/*
 * JUMP INSTRUCTIONS
 * Jumps can never be converted to a superinstruction, as their operand
 * is not part of the instruction itself. Superinstruction concatenation
 * happens purely based on opcode, so with the jump target unknown
 * no meaningful concatenation can be made (that is,
 * control flow cannot stay within the superinstruction).
 *
 * All jump instructions respond to a "WITH_PROFILE" macro value, reporting
 * the branch when taken.
 */


// (+4/+8,-2) or jump
#define INSTR_if_icmpeq(pc,offset)                    \
  const bool cmp = (STACK_INT(-2) == STACK_INT(-1));\
  /* Profile branch. */                               \
  BI_PROFILE_UPDATE_BRANCH(/*is_taken=*/cmp);         \
  if (cmp) {                                          \
    PROFILE_CONDITIONAL(WITH_PROFILE, (pc)+4);        \
    int skip = (int16_t)Bytes::get_Java_u2((pc)+2);   \
    address branch_pc = (pc);                         \
    SET_PC_AND_TOS(pc+skip, offset-2);                \
    DO_BACKEDGE_CHECKS(skip, branch_pc);              \
    CONTINUE;                                         \
  }
// (+4/+8,-1) or jump
#define INSTR_ifeq(pc,offset)                         \
  const bool cmp = (STACK_INT(-1) == 0);\
  /* Profile branch. */                               \
  BI_PROFILE_UPDATE_BRANCH(/*is_taken=*/cmp);         \
  if (cmp) {                                          \
    PROFILE_CONDITIONAL(WITH_PROFILE, (pc)+4);        \
    int skip = (int16_t)Bytes::get_Java_u2((pc)+2);   \
    address branch_pc = (pc);                         \
    SET_PC_AND_TOS(pc+skip, offset-1);                \
    DO_BACKEDGE_CHECKS(skip, branch_pc);              \
    CONTINUE;                                         \
  }
// (+4/+8,-2) or jump
#define INSTR_if_icmpne(pc,offset)                    \
  const bool cmp = (STACK_INT(-2) != STACK_INT(-1));\
  /* Profile branch. */                               \
  BI_PROFILE_UPDATE_BRANCH(/*is_taken=*/cmp);         \
  if (cmp) {                                          \
    PROFILE_CONDITIONAL(WITH_PROFILE, (pc)+4);        \
    int skip = (int16_t)Bytes::get_Java_u2((pc)+2);   \
    address branch_pc = (pc);                         \
    SET_PC_AND_TOS(pc+skip, offset-2);                \
    DO_BACKEDGE_CHECKS(skip, branch_pc);              \
    CONTINUE;                                         \
  }
// (+4/+8,-1) or jump
#define INSTR_ifne(pc,offset)                         \
  const bool cmp = (STACK_INT(-1) != 0);\
  /* Profile branch. */                               \
  BI_PROFILE_UPDATE_BRANCH(/*is_taken=*/cmp);         \
  if (cmp) {                                          \
    PROFILE_CONDITIONAL(WITH_PROFILE, (pc)+4);        \
    int skip = (int16_t)Bytes::get_Java_u2((pc)+2);   \
    address branch_pc = (pc);                         \
    SET_PC_AND_TOS(pc+skip, offset-1);                \
    DO_BACKEDGE_CHECKS(skip, branch_pc);              \
    CONTINUE;                                         \
  }
// (+4/+8,-2) or jump
#define INSTR_if_icmpgt(pc,offset)                    \
  const bool cmp = (STACK_INT(-2) > STACK_INT(-1));\
  /* Profile branch. */                               \
  BI_PROFILE_UPDATE_BRANCH(/*is_taken=*/cmp);         \
  if (cmp) {                                          \
    PROFILE_CONDITIONAL(WITH_PROFILE, (pc)+4);        \
    int skip = (int16_t)Bytes::get_Java_u2((pc)+2);   \
    address branch_pc = (pc);                         \
    SET_PC_AND_TOS(pc+skip, offset-2);                \
    DO_BACKEDGE_CHECKS(skip, branch_pc);              \
    CONTINUE;                                         \
  }
// (+4/+8,-1) or jump
#define INSTR_ifgt(pc,offset)                        \
  const bool cmp = (STACK_INT(-1) > 0);\
  /* Profile branch. */                               \
  BI_PROFILE_UPDATE_BRANCH(/*is_taken=*/cmp);         \
  if (cmp) {                                          \
    PROFILE_CONDITIONAL(WITH_PROFILE, (pc)+4);        \
    int skip = (int16_t)Bytes::get_Java_u2((pc)+2);   \
    address branch_pc = (pc);                         \
    SET_PC_AND_TOS(pc+skip, offset-1);                \
    DO_BACKEDGE_CHECKS(skip, branch_pc);              \
    CONTINUE;                                         \
  }
// (+4/+8,-2) or jump
#define INSTR_if_icmplt(pc,offset)                    \
  const bool cmp = (STACK_INT(-2) < STACK_INT(-1));\
  /* Profile branch. */                               \
  BI_PROFILE_UPDATE_BRANCH(/*is_taken=*/cmp);         \
  if (cmp) {                                          \
    PROFILE_CONDITIONAL(WITH_PROFILE, (pc)+4);        \
    int skip = (int16_t)Bytes::get_Java_u2((pc)+2);   \
    address branch_pc = (pc);                         \
    SET_PC_AND_TOS(pc+skip, offset-2);                \
    DO_BACKEDGE_CHECKS(skip, branch_pc);              \
    CONTINUE;                                         \
  }
// (+4/+8,-1) or jump
#define INSTR_iflt(pc,offset)                        \
  const bool cmp = (STACK_INT(-1) < 0);\
  /* Profile branch. */                               \
  BI_PROFILE_UPDATE_BRANCH(/*is_taken=*/cmp);         \
  if (cmp) {                                          \
    PROFILE_CONDITIONAL(WITH_PROFILE, (pc)+4);        \
    int skip = (int16_t)Bytes::get_Java_u2((pc)+2);   \
    address branch_pc = (pc);                         \
    SET_PC_AND_TOS(pc+skip, offset-1);                \
    DO_BACKEDGE_CHECKS(skip, branch_pc);              \
    CONTINUE;                                         \
  }
// (+4/+8,-2) or jump
#define INSTR_if_icmple(pc,offset)                    \
  const bool cmp = (STACK_INT(-2) <= STACK_INT(-1));\
  /* Profile branch. */                               \
  BI_PROFILE_UPDATE_BRANCH(/*is_taken=*/cmp);         \
  if (cmp) {                                          \
    PROFILE_CONDITIONAL(WITH_PROFILE, (pc)+4);        \
    int skip = (int16_t)Bytes::get_Java_u2((pc)+2);   \
    address branch_pc = (pc);                         \
    SET_PC_AND_TOS(pc+skip, offset-2);                \
    DO_BACKEDGE_CHECKS(skip, branch_pc);              \
    CONTINUE;                                         \
  }
// (+4/+8,-1) or jump
#define INSTR_ifle(pc,offset)                        \
  const bool cmp = (STACK_INT(-1) <= 0);\
  /* Profile branch. */                               \
  BI_PROFILE_UPDATE_BRANCH(/*is_taken=*/cmp);         \
  if (cmp) {                                          \
    PROFILE_CONDITIONAL(WITH_PROFILE, (pc)+4);        \
    int skip = (int16_t)Bytes::get_Java_u2((pc)+2);   \
    address branch_pc = (pc);                         \
    SET_PC_AND_TOS(pc+skip, offset-1);                \
    DO_BACKEDGE_CHECKS(skip, branch_pc);              \
    CONTINUE;                                         \
  }
// (+4/+8,-2) or jump
#define INSTR_if_icmpge(pc,offset)                    \
  const bool cmp = (STACK_INT(-2) >= STACK_INT(-1));\
  /* Profile branch. */                               \
  BI_PROFILE_UPDATE_BRANCH(/*is_taken=*/cmp);         \
  if (cmp) {                                          \
    PROFILE_CONDITIONAL(WITH_PROFILE, (pc)+4);        \
    int skip = (int16_t)Bytes::get_Java_u2((pc)+2);   \
    address branch_pc = (pc);                         \
    SET_PC_AND_TOS(pc+skip, offset-2);                \
    DO_BACKEDGE_CHECKS(skip, branch_pc);              \
    CONTINUE;                                         \
  }
// (+4/+8,-1) or jump
#define INSTR_ifge(pc,offset)                        \
  const bool cmp = (STACK_INT(-1) >= 0);\
  /* Profile branch. */                               \
  BI_PROFILE_UPDATE_BRANCH(/*is_taken=*/cmp);         \
  if (cmp) {                                          \
    PROFILE_CONDITIONAL(WITH_PROFILE, (pc)+4);        \
    int skip = (int16_t)Bytes::get_Java_u2((pc)+2);   \
    address branch_pc = (pc);                         \
    SET_PC_AND_TOS(pc+skip, offset-1);                \
    DO_BACKEDGE_CHECKS(skip, branch_pc);              \
    CONTINUE;                                         \
  }

// (+4/+8,-2) or jump
#define INSTR_if_acmpeq(pc,offset)                    \
  const bool cmp = (STACK_OBJECT(-2) == STACK_OBJECT(-1));\
  /* Profile branch. */                               \
  BI_PROFILE_UPDATE_BRANCH(/*is_taken=*/cmp);         \
  if (cmp) {                                          \
    PROFILE_CONDITIONAL(WITH_PROFILE, (pc)+4);        \
    int skip = (int16_t)Bytes::get_Java_u2((pc)+2);   \
    address branch_pc = (pc);                         \
    SET_PC_AND_TOS(pc+skip, offset-2);                \
    DO_BACKEDGE_CHECKS(skip, branch_pc);              \
    CONTINUE;                                         \
  }
// (+4/+8,-2) or jump
#define INSTR_if_acmpne(pc,offset)                    \
  const bool cmp = (STACK_OBJECT(-2) != STACK_OBJECT(-1));\
  /* Profile branch. */                                \
  BI_PROFILE_UPDATE_BRANCH(/*is_taken=*/cmp);          \
  if (cmp) {                                           \
    PROFILE_CONDITIONAL(WITH_PROFILE, (pc)+4);         \
    int skip = (int16_t)Bytes::get_Java_u2((pc)+2);    \
    address branch_pc = (pc);                          \
    SET_PC_AND_TOS(pc+skip, offset-2);                 \
    DO_BACKEDGE_CHECKS(skip, branch_pc);               \
    CONTINUE;                                          \
  }

// (+4/+8,-1) or jump
#define INSTR_ifnull(pc,offset)                       \
  const bool cmp = (STACK_OBJECT(-1) == NULL);        \
  /* Profile branch. */                               \
  BI_PROFILE_UPDATE_BRANCH(/*is_taken=*/cmp);         \
  if (cmp) {                                          \
    PROFILE_CONDITIONAL(WITH_PROFILE, (pc)+4);        \
    int skip = (int16_t)Bytes::get_Java_u2((pc)+2);   \
    address branch_pc = (pc);                         \
    SET_PC_AND_TOS(pc+skip, offset-1);                \
    DO_BACKEDGE_CHECKS(skip, branch_pc);              \
    CONTINUE;                                         \
  }

// (+4/+8,-1) or jump
#define INSTR_ifnonnull(pc,offset)                    \
  const bool cmp = (!(STACK_OBJECT(-1) == NULL));     \
  /* Profile branch. */                               \
  BI_PROFILE_UPDATE_BRANCH(/*is_taken=*/cmp);         \
  if (cmp) {                                          \
    PROFILE_CONDITIONAL(WITH_PROFILE, (pc)+4);        \
    int skip = (int16_t)Bytes::get_Java_u2((pc)+2);   \
    address branch_pc = (pc);                         \
    SET_PC_AND_TOS(pc+skip, offset-1);                \
    DO_BACKEDGE_CHECKS(skip, branch_pc);              \
    CONTINUE;                                         \
  }

// TODO: tableswitch and lookupswitch
// These instructions will require some thought for profiling

// (+?,+?) unconditional jump
#define INSTR_tableswitch(pc,offset)                  \
  jint* lpc  = (jint*)VMalignWordUp((pc)+2);          \
  int32_t  key  = STACK_INT(offset-1);                \
  int32_t  low  = Bytes::get_Java_u4((address)&lpc[1]);\
  int32_t  high = Bytes::get_Java_u4((address)&lpc[2]);\
  int32_t  skip;                                      \
  key -= low;                                         \
  if (((uint32_t) key > (uint32_t)(high - low))) {    \
    key = -1;                                         \
    skip = Bytes::get_Java_u4((address)&lpc[0]);      \
  } else {                                            \
    skip = Bytes::get_Java_u4((address)&lpc[key + 3]);\
  }                                                   \
  BI_PROFILE_UPDATE_SWITCH(/*switch_index=*/key);     \
  address branch_pc = pc;                             \
  UPDATE_PC_AND_TOS(skip, offset-1);                  \
  DO_BACKEDGE_CHECKS(skip, branch_pc);                \
  CONTINUE;

// (+?,+?) unconditional jump
#define INSTR_lookupswitch(pc,offset)                 \
  jint* lpc  = (jint*)VMalignWordUp((pc)+2);          \
  int32_t  key  = STACK_INT(offset-1);                \
  int32_t  skip = Bytes::get_Java_u4((address) lpc); /* default amount */\
  /* Remember index. */                               \
  int      index = -1;                                \
  int      newindex = 0;                              \
  int32_t  npairs = Bytes::get_Java_u4((address) &lpc[1]);\
  while (--npairs >= 0) {                             \
    lpc += 2;                                         \
    if (key == (int32_t)Bytes::get_Java_u4((address)lpc)) {\
      skip = Bytes::get_Java_u4((address)&lpc[1]);    \
      index = newindex;                               \
      break;                                          \
    }                                                 \
    newindex += 1;                                    \
  }                                                   \
  /*= Profile switch. */                              \
  BI_PROFILE_UPDATE_SWITCH(/*switch_index=*/index);   \
  address branch_pc = pc;                             \
  UPDATE_PC_AND_TOS(skip, offset-1);                  \
  DO_BACKEDGE_CHECKS(skip, branch_pc);                \
  CONTINUE;

// (+2,-1)
#define INSTR_fcmpl(pc,offset)                        \
  SET_STACK_INT(VMfloatCompare(STACK_FLOAT(offset-2), \
                               STACK_FLOAT(offset-1), \
                               -1),                   \
                               offset-2);

// (+2,-1)
#define INSTR_fcmpg(pc,offset)                        \
  SET_STACK_INT(VMfloatCompare(STACK_FLOAT(offset-2), \
                               STACK_FLOAT(offset-1), \
                               1),                    \
                               offset-2);

// (+2,-3)
#define INSTR_dcmpl(pc,offset)                        \
  int r = VMdoubleCompare(STACK_DOUBLE(offset-3),     \
                          STACK_DOUBLE(offset-1),     \
                          -1);                        \
  SET_STACK_INT(r, offset-4);

// (+2,-3)
#define INSTR_dcmpg(pc,offset)                        \
  int r = VMdoubleCompare(STACK_DOUBLE(offset-3),     \
                          STACK_DOUBLE(offset-1),     \
                          1);                         \
  SET_STACK_INT(r, offset-4);

// (+2,-3)
#define INSTR_lcmp(pc,offset)                         \
  int r = VMlongCompare(STACK_LONG(offset-3),         \
                        STACK_LONG(offset-1));        \
  SET_STACK_INT(r, offset-4);

// (+?,?) terminal safepoint (all)
#define INSTR_areturn(pc,offset)                      \
  SAFEPOINT;                                          \
  goto handle_return;
#define INSTR_ireturn(pc,offset)                      \
  SAFEPOINT;                                          \
  goto handle_return;
#define INSTR_freturn(pc,offset)                      \
  SAFEPOINT;                                          \
  goto handle_return;
#define INSTR_lreturn(pc,offset)                      \
  SAFEPOINT;                                          \
  goto handle_return;
#define INSTR_dreturn(pc,offset)                      \
  SAFEPOINT;                                          \
  goto handle_return;
#define INSTR_return(pc,offset)                       \
  SAFEPOINT;                                          \
  goto handle_return;

// TODO: what to do with return_register_finalizers?
// (+?,?) terminal
#define INSTR_return_register_finalizer(pc,offset)    \
  oop rcvr = LOCALS_OBJECT(offset);                   \
  VERIFY_OOP(rcvr);                                   \
  if (rcvr->klass()->has_finalizer()) {               \
    CALL_VM(InterpreterRuntime::register_finalizer(THREAD, rcvr), handle_exception);\
  }                                                   \
  goto handle_return;

/* Every array access byte-code starts out like this */
#define ARRAY_INTRO(arrayOff)                                              \
  arrayOop arrObj = (arrayOop)STACK_OBJECT(arrayOff);                      \
  jint     index  = STACK_INT(arrayOff + 1);                               \
  char message[jintAsStringSize];                                          \
  CHECK_NULL(arrObj);                                                      \
  if ((uint32_t)index >= (uint32_t)arrObj->length()) {                     \
      sprintf(message, "%d", index);                                       \
      VM_JAVA_ERROR(vmSymbols::java_lang_ArrayIndexOutOfBoundsException(), \
                    message, note_rangeCheck_trap);                        \
  }

/* 32-bit loads. These handle conversion from < 32-bit types, (+2,-1) */
#define ARRAY_LOADTO32(T, T2, format, stackRes, extra, offset)                    \
  {                                                                               \
    ARRAY_INTRO(offset-2);                                                      \
    (void)extra;                                                                \
    SET_ ## stackRes(*(T2 *)(((address) arrObj->base(T)) + index * sizeof(T2)), \
                     offset-2);                                                 \
  }

/* 64-bit loads, (+2,0) */
#define ARRAY_LOADTO64(T,T2, stackRes, extra, offset)                                  \
  {                                                                                    \
    ARRAY_INTRO(offset-2);                                                           \
    SET_ ## stackRes(*(T2 *)(((address) arrObj->base(T)) + index * sizeof(T2)),      \
      offset-1);                                                                     \
    (void)extra;                                                                     \
  }

// (+2,-1) (all)
#define INSTR_iaload(pc,offset)                         \
  ARRAY_LOADTO32(T_INT, jint, "%d", STACK_INT, 0, offset);
#define INSTR_faload(pc,offset)                         \
  ARRAY_LOADTO32(T_FLOAT, jfloat, "%f", STACK_FLOAT, 0, offset);
#define INSTR_aaload(pc,offset)                         \
  ARRAY_INTRO(offset-2);                                \
  SET_STACK_OBJECT(((objArrayOop) arrObj)->obj_at(index), offset-2);
#define INSTR_baload(pc,offset)                         \
  ARRAY_LOADTO32(T_BYTE, jbyte, "%d", STACK_INT, 0, offset);
#define INSTR_caload(pc,offset)                         \
  ARRAY_LOADTO32(T_CHAR, jchar, "%d", STACK_INT, 0, offset);
#define INSTR_saload(pc,offset)                         \
  ARRAY_LOADTO32(T_SHORT, jshort, "%d", STACK_INT, 0, offset);

// (+2,0) (all)
#define INSTR_laload(pc,offset)                         \
  ARRAY_LOADTO64(T_LONG, jlong, STACK_LONG, 0, offset);
#define INSTR_daload(pc,offset)                         \
  ARRAY_LOADTO64(T_DOUBLE, jdouble, STACK_DOUBLE, 0, offset);

/* 32-bit stores. These handle conversion to < 32-bit types, (+2,-3) */
#define ARRAY_STOREFROM32(T, T2, format, stackSrc, extra, offset)              \
  {                                                                            \
    ARRAY_INTRO(offset-3);                                                     \
    (void)extra;                                                               \
    *(T2 *)(((address) arrObj->base(T)) + index * sizeof(T2)) = stackSrc(offset-1);\
  }

/* 64-bit stores, (+2,-4) */
#define ARRAY_STOREFROM64(T, T2, stackSrc, extra, offset)                      \
  {                                                                            \
    ARRAY_INTRO(offset-4);                                                     \
    (void)extra;                                                               \
    *(T2 *)(((address) arrObj->base(T)) + index * sizeof(T2)) = stackSrc(offset-1);\
  }


// (+2,-3) (all)
#define INSTR_iastore(pc,offset)                      \
  ARRAY_STOREFROM32(T_INT, jint, "%d", STACK_INT, 0, offset);
#define INSTR_fastore(pc,offset)                      \
  ARRAY_STOREFROM32(T_FLOAT, jfloat, "%f", STACK_FLOAT, 0, offset);
#define INSTR_aastore(pc,offset)                      \
  oop rhsObject = STACK_OBJECT(offset-1);             \
  VERIFY_OOP(rhsObject);                              \
  ARRAY_INTRO(offset-3);                              \
  if (rhsObject != NULL) {                            \
    /* Check assignability of rhsObject into arrObj */\
    Klass* rhsKlass = rhsObject->klass();             \
    Klass* elemKlass = ObjArrayKlass::cast(arrObj->klass())->element_klass();\
    /*
      Check for compatibilty. This check must not GC!!
      Seems way more expensive now that we must dispatch
    */                                                \
    if (rhsKlass != elemKlass && !rhsKlass->is_subtype_of(elemKlass)) {\
      BI_PROFILE_SUBTYPECHECK_FAILED(rhsKlass);       \
      SET_PC_AND_TOS(pc,offset);                      \
      VM_JAVA_ERROR(vmSymbols::java_lang_ArrayStoreException(), "", note_arrayCheck_trap);\
    }                                                 \
    BI_PROFILE_UPDATE_CHECKCAST(/*null_seen=*/false, rhsKlass);\
  } else {                                            \
    BI_PROFILE_UPDATE_CHECKCAST(/*null_seen=*/true, NULL);\
  }                                                   \
  ((objArrayOop) arrObj)->obj_at_put(index, rhsObject);
#define INSTR_bastore(pc,offset)                      \
  ARRAY_INTRO(offset-3);                              \
  int item = STACK_INT(offset-1);                     \
  if (arrObj->klass() == Universe::boolArrayKlassObj()) {\
    item &= 1;                                        \
  } else {                                            \
    assert(arrObj->klass() == Universe::byteArrayKlassObj(),\
           "should be byte array otherwise");         \
  }                                                   \
  ((typeArrayOop)arrObj)->byte_at_put(index, item);
#define INSTR_castore(pc,offset)                      \
  ARRAY_STOREFROM32(T_CHAR, jchar,  "%d",   STACK_INT, 0, offset);
#define INSTR_sastore(pc,offset)                      \
  ARRAY_STOREFROM32(T_SHORT, jshort, "%d",   STACK_INT, 0, offset);

// (+2,-4)
#define INSTR_lastore(pc,offset)                      \
  ARRAY_STOREFROM64(T_LONG, jlong, STACK_LONG, 0, offset);
#define INSTR_dastore(pc,offset)                      \
  ARRAY_STOREFROM64(T_DOUBLE, jdouble, STACK_DOUBLE, 0, offset);

// (+2,0)
#define INSTR_arraylength(pc,offset)                  \
  arrayOop ary = (arrayOop) STACK_OBJECT(offset-1);   \
  CHECK_NULL(ary);                                    \
  SET_STACK_INT(ary->length(), offset-1);

// no superinstruction
#define INSTR_monitorenter(pc,offset)                 \
  oop lockee = STACK_OBJECT(offset-1);                \
  /* TODO: fix null check offset */                   \
  CHECK_NULL(lockee);                                 \
  BasicObjectLock* limit = istate->monitor_base();    \
  BasicObjectLock* most_recent = (BasicObjectLock*) istate->stack_base();\
  BasicObjectLock* entry = NULL;                      \
  while (most_recent != limit ) {                     \
    if (most_recent->obj() == NULL) entry = most_recent;\
    else if (most_recent->obj() == lockee) break;     \
    most_recent++;                                    \
  }                                                   \
  if (entry != NULL) {                                \
    entry->set_obj(lockee);                           \
    int success = false;                              \
    uintptr_t epoch_mask_in_place = (uintptr_t)markOopDesc::epoch_mask_in_place;\
    markOop mark = lockee->mark();                    \
    intptr_t hash = (intptr_t) markOopDesc::no_hash;  \
    if (mark->has_bias_pattern()) {                   \
      uintptr_t thread_ident;                         \
      uintptr_t anticipated_bias_locking_value;       \
      thread_ident = (uintptr_t)istate->thread();     \
      anticipated_bias_locking_value =                \
        (((uintptr_t)lockee->klass()->prototype_header() | thread_ident) ^ (uintptr_t)mark) &\
        ~((uintptr_t) markOopDesc::age_mask_in_place);\
                                                      \
      if  (anticipated_bias_locking_value == 0) {     \
        if (PrintBiasedLockingStatistics) {           \
          (* BiasedLocking::biased_lock_entry_count_addr())++;\
        }                                             \
        success = true;                               \
      }                                               \
      else if ((anticipated_bias_locking_value & markOopDesc::biased_lock_mask_in_place) != 0) {\
        markOop header = lockee->klass()->prototype_header();\
        if (hash != markOopDesc::no_hash) {           \
          header = header->copy_set_hash(hash);       \
        }                                             \
        if (lockee->cas_set_mark(header, mark) == mark) {\
          if (PrintBiasedLockingStatistics)           \
            (*BiasedLocking::revoked_lock_entry_count_addr())++;\
        }                                             \
      }                                               \
      else if ((anticipated_bias_locking_value & epoch_mask_in_place) !=0) {\
        markOop new_header = (markOop) ( (intptr_t) lockee->klass()->prototype_header() | thread_ident);\
        if (hash != markOopDesc::no_hash) {           \
          new_header = new_header->copy_set_hash(hash);\
        }                                             \
        if (lockee->cas_set_mark(new_header, mark) == mark) {\
          if (PrintBiasedLockingStatistics)           \
            (* BiasedLocking::rebiased_lock_entry_count_addr())++;\
        }                                             \
        else {                                        \
          CALL_VM(InterpreterRuntime::monitorenter(THREAD, entry), handle_exception);\
        }                                             \
        success = true;                               \
      }                                               \
      else {                                          \
        markOop header = (markOop) ((uintptr_t) mark & ((uintptr_t)markOopDesc::biased_lock_mask_in_place |\
                                                        (uintptr_t)markOopDesc::age_mask_in_place |\
                                                        epoch_mask_in_place));\
        if (hash != markOopDesc::no_hash) {           \
          header = header->copy_set_hash(hash);       \
        }                                             \
        markOop new_header = (markOop) ((uintptr_t) header | thread_ident);\
        DEBUG_ONLY(entry->lock()->set_displaced_header((markOop) (uintptr_t) 0xdeaddead);)\
        if (lockee->cas_set_mark(new_header, header) == header) {\
          if (PrintBiasedLockingStatistics)           \
            (* BiasedLocking::anonymously_biased_lock_entry_count_addr())++;\
        }                                             \
        else {                                        \
          CALL_VM(InterpreterRuntime::monitorenter(THREAD, entry), handle_exception);\
        }                                             \
        success = true;                               \
      }                                               \
    }                                                 \
                                                      \
    if (!success) {                                   \
      markOop displaced = lockee->mark()->set_unlocked();\
      entry->lock()->set_displaced_header(displaced); \
      bool call_vm = UseHeavyMonitors;                \
      if (call_vm || lockee->cas_set_mark((markOop)entry, displaced) != displaced) {\
        if (!call_vm && THREAD->is_lock_owned((address) displaced->clear_lock_bits())) {\
          entry->lock()->set_displaced_header(NULL);  \
        } else {                                      \
          CALL_VM(InterpreterRuntime::monitorenter(THREAD, entry), handle_exception);\
        }                                             \
      }                                               \
    }                                                 \
    UPDATE_PC_AND_TOS_AND_CONTINUE(2, -1);            \
  } else {                                            \
    istate->set_msg(more_monitors);                   \
    UPDATE_PC_AND_RETURN(0); /* Re-execute */         \
  }

// (+2,-1)
#define INSTR_monitorexit(pc,offset)                  \
  oop lockee = STACK_OBJECT(offset-1);                \
  CHECK_NULL(lockee);                                 \
  BasicObjectLock* limit = istate->monitor_base();    \
  BasicObjectLock* most_recent = (BasicObjectLock*) istate->stack_base();\
  while (most_recent != limit ) {                     \
    if ((most_recent)->obj() == lockee) {             \
      BasicLock* lock = most_recent->lock();          \
      markOop header = lock->displaced_header();      \
      most_recent->set_obj(NULL);                     \
      if (!lockee->mark()->has_bias_pattern()) {      \
        bool call_vm = UseHeavyMonitors;              \
        if (header != NULL || call_vm) {              \
          markOop old_header = markOopDesc::encode(lock);\
          if (call_vm || lockee->cas_set_mark(header, old_header) != old_header) {\
            most_recent->set_obj(lockee);             \
            CALL_VM(InterpreterRuntime::monitorexit(THREAD, most_recent), handle_exception);\
          }                                           \
        }                                             \
      }                                               \
      break;                                          \
    }                                                 \
    most_recent++;                                    \
  }                                                   \
  if (most_recent == limit) {                         \
      CALL_VM(InterpreterRuntime::throw_illegal_monitor_state_exception(THREAD), handle_exception);\
      ShouldNotReachHere();                           \
  }

// (+4,?) terminal, manipulates-tos
#define INSTR_getstatic_or_getfield(pc,offset,is_getstatic)\
  u2 index;                                           \
  ConstantPoolCacheEntry* cache;                      \
  index = Bytes::get_native_u2((pc)+2);               \
  MORE_STACK(offset); /* flush offset */              \
                                                      \
  cache = cp->entry_at(index);                        \
  if (!cache->is_resolved((Bytecodes::Code)opcode)) { \
    CALL_VM(InterpreterRuntime::resolve_from_cache(THREAD, (Bytecodes::Code)opcode),\
            handle_exception);                        \
    cache = cp->entry_at(index);                      \
  }                                                   \
                                                      \
  oop obj;                                            \
  if (is_getstatic) {                                 \
    Klass* k = cache->f1_as_klass();                  \
    obj = k->java_mirror();                           \
    MORE_STACK(1);                                    \
  } else {                                            \
    obj = (oop) STACK_OBJECT(-1);                     \
    CHECK_NULL(obj);                                  \
  }                                                   \
                                                      \
  TosState tos_type = cache->flag_state();            \
  int field_offset = cache->f2_as_index();            \
  if (cache->is_volatile()) {                         \
    if (support_IRIW_for_not_multiple_copy_atomic_cpu) {\
      OrderAccess::fence();                           \
    }                                                 \
    if (tos_type == atos) {                           \
      VERIFY_OOP(obj->obj_field_acquire(field_offset));\
      SET_STACK_OBJECT(obj->obj_field_acquire(field_offset), -1);\
    } else if (tos_type == itos) {                    \
      SET_STACK_INT(obj->int_field_acquire(field_offset), -1);\
    } else if (tos_type == ltos) {                    \
      SET_STACK_LONG(obj->long_field_acquire(field_offset), 0);\
      MORE_STACK(1);                                  \
    } else if (tos_type == btos || tos_type == ztos) {\
      SET_STACK_INT(obj->byte_field_acquire(field_offset), -1);\
    } else if (tos_type == ctos) {                    \
      SET_STACK_INT(obj->char_field_acquire(field_offset), -1);\
    } else if (tos_type == stos) {                    \
      SET_STACK_INT(obj->short_field_acquire(field_offset), -1);\
    } else if (tos_type == ftos) {                    \
      SET_STACK_FLOAT(obj->float_field_acquire(field_offset), -1);\
    } else {                                          \
      SET_STACK_DOUBLE(obj->double_field_acquire(field_offset), 0);\
      MORE_STACK(1);                                  \
    }                                                 \
  } else {                                            \
    if (tos_type == atos) {                           \
      VERIFY_OOP(obj->obj_field(field_offset));       \
      SET_STACK_OBJECT(obj->obj_field(field_offset), -1);\
    } else if (tos_type == itos) {                    \
      SET_STACK_INT(obj->int_field(field_offset), -1);\
    } else if (tos_type == ltos) {                    \
      SET_STACK_LONG(obj->long_field(field_offset), 0);\
      MORE_STACK(1);                                  \
    } else if (tos_type == btos || tos_type == ztos) {\
      SET_STACK_INT(obj->byte_field(field_offset), -1);\
    } else if (tos_type == ctos) {                    \
      SET_STACK_INT(obj->char_field(field_offset), -1);\
    } else if (tos_type == stos) {                    \
      SET_STACK_INT(obj->short_field(field_offset), -1);\
    } else if (tos_type == ftos) {                    \
      SET_STACK_FLOAT(obj->float_field(field_offset), -1);\
    } else {                                          \
      SET_STACK_DOUBLE(obj->double_field(field_offset), 0);\
      MORE_STACK(1);                                  \
    }                                                 \
  }

// (+4,?) manipulates-tos
#define INSTR_getfield(pc,offset)                     \
  INSTR_getstatic_or_getfield(pc,offset,false);

// (+4,?) manipulates-tos
#define INSTR_getstatic(pc,offset)                    \
  INSTR_getstatic_or_getfield(pc,offset,true);

// (+4,?) manipulates-tos
#define INSTR_putfield_or_putstatic(pc,offset,is_putstatic)\
  u2 index = Bytes::get_native_u2((pc)+2);            \
  ConstantPoolCacheEntry* cache = cp->entry_at(index);\
  if (!cache->is_resolved((Bytecodes::Code)opcode)) { \
    CALL_VM(InterpreterRuntime::resolve_from_cache(THREAD, (Bytecodes::Code)opcode),\
            handle_exception);                        \
    cache = cp->entry_at(index);                      \
  }                                                   \
                                                      \
  oop obj;                                            \
  int count;                                          \
  TosState tos_type = cache->flag_state();            \
                                                      \
  count = -1;                                         \
  if (tos_type == ltos || tos_type == dtos) {         \
    --count;                                          \
  }                                                   \
  if (is_putstatic) {                                 \
    Klass* k = cache->f1_as_klass();                  \
    obj = k->java_mirror();                           \
  } else {                                            \
    --count;                                          \
    obj = (oop) STACK_OBJECT(count);                  \
    CHECK_NULL(obj);                                  \
  }                                                   \
                                                      \
  int field_offset = cache->f2_as_index();            \
  if (cache->is_volatile()) {                         \
    if (tos_type == itos) {                           \
      obj->release_int_field_put(field_offset, STACK_INT(offset-1));\
    } else if (tos_type == atos) {                    \
      VERIFY_OOP(STACK_OBJECT(offset-1));             \
      obj->release_obj_field_put(field_offset, STACK_OBJECT(offset-1));\
    } else if (tos_type == btos) {                    \
      obj->release_byte_field_put(field_offset, STACK_INT(offset-1));\
    } else if (tos_type == ztos) {                    \
      int bool_field = STACK_INT(offset-1);           \
      obj->release_byte_field_put(field_offset, (bool_field & 1));\
    } else if (tos_type == ltos) {                    \
      obj->release_long_field_put(field_offset, STACK_LONG(offset-1));\
    } else if (tos_type == ctos) {                    \
      obj->release_char_field_put(field_offset, STACK_INT(offset-1));\
    } else if (tos_type == stos) {                    \
      obj->release_short_field_put(field_offset, STACK_INT(offset-1));\
    } else if (tos_type == ftos) {                    \
      obj->release_float_field_put(field_offset, STACK_FLOAT(offset-1));\
    } else {                                          \
      obj->release_double_field_put(field_offset, STACK_DOUBLE(offset-1));\
    }                                                 \
    OrderAccess::storeload();                         \
  } else {                                            \
    if (tos_type == itos) {                           \
      obj->int_field_put(field_offset, STACK_INT(offset-1));\
    } else if (tos_type == atos) {                    \
      VERIFY_OOP(STACK_OBJECT(offset-1));             \
      obj->obj_field_put(field_offset, STACK_OBJECT(offset-1));\
    } else if (tos_type == btos) {                    \
      obj->byte_field_put(field_offset, STACK_INT(offset-1));\
    } else if (tos_type == ztos) {                    \
      int bool_field = STACK_INT(offset-1);           \
      obj->byte_field_put(field_offset, (bool_field & 1));\
    } else if (tos_type == ltos) {                    \
      obj->long_field_put(field_offset, STACK_LONG(offset-1));\
    } else if (tos_type == ctos) {                    \
      obj->char_field_put(field_offset, STACK_INT(offset-1));\
    } else if (tos_type == stos) {                    \
      obj->short_field_put(field_offset, STACK_INT(offset-1));\
    } else if (tos_type == ftos) {                    \
      obj->float_field_put(field_offset, STACK_FLOAT(offset-1));\
    } else {                                          \
      obj->double_field_put(field_offset, STACK_DOUBLE(offset-1));\
    }                                                 \
  }                                                   \
  MORE_STACK(count+offset); /* flush offset */

// (+4,?) manipulates-tos
#define INSTR_putfield(pc,offset)                     \
  INSTR_putfield_or_putstatic(pc,offset,false);

// (+4,?) manipulates-tos
#define INSTR_putstatic(pc,offset)                    \
  INSTR_putfield_or_putstatic(pc,offset,true);

// (+4,+1)
#define INSTR_new(pc,offset)                          \
  u2 index = Bytes::get_Java_u2((pc)+2);              \
  bool fastpath_allocated = false;                    \
  ConstantPool* constants = istate->method()->constants();\
  if (!constants->tag_at(index).is_unresolved_klass()) {\
    Klass* entry = constants->resolved_klass_at(index);\
    InstanceKlass* ik = InstanceKlass::cast(entry);   \
    if (ik->is_initialized() && ik->can_be_fastpath_allocated() ) {\
      size_t obj_size = ik->size_helper();            \
      oop result = NULL;                              \
      bool need_zero = !ZeroTLAB;                     \
      if (UseTLAB) {                                  \
        result = (oop) THREAD->tlab().allocate(obj_size);\
      }                                               \
      if (result != NULL) {                           \
        if (need_zero ) {                             \
          HeapWord* to_zero = (HeapWord*) result + sizeof(oopDesc) / oopSize;\
          obj_size -= sizeof(oopDesc) / oopSize;      \
          if (obj_size > 0 ) {                        \
            memset(to_zero, 0, obj_size * HeapWordSize);\
          }                                           \
        }                                             \
        if (UseBiasedLocking) {                       \
          result->set_mark(ik->prototype_header());   \
        } else {                                      \
          result->set_mark(markOopDesc::prototype()); \
        }                                             \
        result->set_klass_gap(0);                     \
        result->set_klass(ik);                        \
        OrderAccess::storestore();                    \
        SET_STACK_OBJECT(result, offset);             \
        fastpath_allocated = true;                    \
      }                                               \
    }                                                 \
  }                                                   \
  if (!fastpath_allocated) {                          \
    /* Slow case allocation */                        \
    CALL_VM(InterpreterRuntime::_new(THREAD, METHOD->constants(), index),\
            handle_exception);                        \
    OrderAccess::storestore();                        \
    SET_STACK_OBJECT(THREAD->vm_result(), offset);    \
    THREAD->set_vm_result(NULL);                      \
  }

// (+4,0)
#define INSTR_anewarray(pc,offset)                    \
  u2 index = Bytes::get_Java_u2((pc)+2);              \
  jint size = STACK_INT(offset-1);                    \
  CALL_VM(InterpreterRuntime::anewarray(THREAD, METHOD->constants(), index, size),\
          handle_exception);                          \
  OrderAccess::storestore();                          \
  SET_STACK_OBJECT(THREAD->vm_result(), -1);          \
  THREAD->set_vm_result(NULL);

// (+4,+?) updates-tos
#define INSTR_multianewarray(pc,offset)               \
  jint dims = *((pc)+4);                              \
  jint size = STACK_INT(offset-1);                    \
  /* stack grows down, dimensions are up! */          \
  jint *dimarray =                                    \
             (jint*)&topOfStack[dims * Interpreter::stackElementWords+\
                                Interpreter::stackElementWords-1];\
  /* adjust pointer to start of stack element */      \
  CALL_VM(InterpreterRuntime::multianewarray(THREAD, dimarray),\
          handle_exception);                          \
  OrderAccess::storestore();                          \
  SET_STACK_OBJECT(THREAD->vm_result(), offset-dims); \
  THREAD->set_vm_result(NULL);                        \
  MORE_STACK(offset - (dims - 1));


// (+4,0)
#define INSTR_checkcast(pc,offset)                    \
  if (STACK_OBJECT(-1) != NULL) {                     \
    VERIFY_OOP(STACK_OBJECT(offset-1));               \
    u2 index = Bytes::get_Java_u2((pc)+2);            \
    if (METHOD->constants()->tag_at(index).is_unresolved_klass()) {\
      CALL_VM(InterpreterRuntime::quicken_io_cc(THREAD), handle_exception);\
    }                                                 \
    Klass* klassOf = (Klass*) METHOD->constants()->resolved_klass_at(index);\
    Klass* objKlass = STACK_OBJECT(offset-1)->klass();\
    if (objKlass != klassOf && !objKlass->is_subtype_of(klassOf)) {\
      BI_PROFILE_SUBTYPECHECK_FAILED(objKlass);       \
      ResourceMark rm(THREAD);                        \
      char* message = SharedRuntime::generate_class_cast_message(\
        objKlass, klassOf);                           \
      VM_JAVA_ERROR(vmSymbols::java_lang_ClassCastException(), message, note_classCheck_trap);\
    }                                                 \
    BI_PROFILE_UPDATE_CHECKCAST(/*null_seen=*/false, objKlass);\
  } else {                                            \
    BI_PROFILE_UPDATE_CHECKCAST(/*null_seen=*/true, NULL);\
  }

// (+4,0)
#define INSTR_instanceof(pc,offset)                   \
  if (STACK_OBJECT(offset-1) == NULL) {               \
    SET_STACK_INT(0, offset-1);                       \
    BI_PROFILE_UPDATE_INSTANCEOF(/*null_seen=*/true, NULL);\
  } else {                                            \
    VERIFY_OOP(STACK_OBJECT(offset-1));               \
    u2 index = Bytes::get_Java_u2((pc)+2);            \
    if (METHOD->constants()->tag_at(index).is_unresolved_klass()) {\
      CALL_VM(InterpreterRuntime::quicken_io_cc(THREAD), handle_exception);\
    }                                                 \
    Klass* klassOf = (Klass*) METHOD->constants()->resolved_klass_at(index);\
    Klass* objKlass = STACK_OBJECT(offset-1)->klass();\
    if ( objKlass == klassOf || objKlass->is_subtype_of(klassOf)) {\
      SET_STACK_INT(1, offset-1);                     \
    } else {                                          \
      SET_STACK_INT(0, offset-1);                     \
      BI_PROFILE_SUBTYPECHECK_FAILED(objKlass);       \
    }                                                 \
    BI_PROFILE_UPDATE_INSTANCEOF(/*null_seen=*/false, objKlass);\
  }

// (+3,1) for non-wide, (+4,1) for wide
#define INSTR_ldc_or_ldc_w(pc,offset,is_wide)         \
  u2 index;                                           \
  if (is_wide) {                                      \
    index = Bytes::get_Java_u2((pc)+2);               \
  } else {                                            \
    index = (pc)[2];                                  \
  }                                                   \
                                                      \
  ConstantPool* constants = METHOD->constants();      \
  switch (constants->tag_at(index).value()) {         \
  case JVM_CONSTANT_Integer:                          \
    SET_STACK_INT(constants->int_at(index), offset);  \
    break;                                            \
                                                      \
  case JVM_CONSTANT_Float:                            \
    SET_STACK_FLOAT(constants->float_at(index), offset);\
    break;                                            \
                                                      \
  case JVM_CONSTANT_String:                           \
    {                                                 \
      oop result = constants->resolved_references()->obj_at(index);\
      if (result == NULL) {                           \
        CALL_VM(InterpreterRuntime::resolve_ldc(THREAD, is_wide ? Bytecodes::_ldc_w : Bytecodes::_ldc), handle_exception);\
        SET_STACK_OBJECT(THREAD->vm_result(), offset);\
        THREAD->set_vm_result(NULL);                  \
      } else {                                        \
        VERIFY_OOP(result);                           \
        SET_STACK_OBJECT(result, offset);             \
      }                                               \
    break;                                            \
    }                                                 \
                                                      \
  case JVM_CONSTANT_Class:                            \
    VERIFY_OOP(constants->resolved_klass_at(index)->java_mirror());\
    SET_STACK_OBJECT(constants->resolved_klass_at(index)->java_mirror(), offset);\
    break;                                            \
                                                      \
  case JVM_CONSTANT_UnresolvedClass:                  \
  case JVM_CONSTANT_UnresolvedClassInError:           \
    CALL_VM(InterpreterRuntime::ldc(THREAD, is_wide), handle_exception);\
    SET_STACK_OBJECT(THREAD->vm_result(), offset);    \
    THREAD->set_vm_result(NULL);                      \
    break;                                            \
                                                      \
  case JVM_CONSTANT_Dynamic:                          \
    {                                                 \
      oop result = constants->resolved_references()->obj_at(index);\
      if (result == NULL) {                           \
        CALL_VM(InterpreterRuntime::resolve_ldc(THREAD,\
                is_wide ? Bytecodes::_ldc_w : Bytecodes::_ldc),\
                handle_exception);                    \
        result = THREAD->vm_result();                 \
      }                                               \
      VERIFY_OOP(result);                             \
                                                      \
      jvalue value;                                   \
      BasicType type = java_lang_boxing_object::get_value(result, &value);\
      switch (type) {                                 \
      case T_FLOAT:   SET_STACK_FLOAT(value.f, offset); break;\
      case T_INT:     SET_STACK_INT(value.i, offset); break;\
      case T_SHORT:   SET_STACK_INT(value.s, offset); break;\
      case T_BYTE:    SET_STACK_INT(value.b, offset); break;\
      case T_CHAR:    SET_STACK_INT(value.c, offset); break;\
      case T_BOOLEAN: SET_STACK_INT(value.z, offset); break;\
      default:  ShouldNotReachHere();                 \
      }                                               \
                                                      \
      break;                                          \
    }                                                 \
                                                      \
  default:  ShouldNotReachHere();                     \
  }

// (+3,+1)
#define INSTR_ldc(pc,offset)                          \
  INSTR_ldc_or_ldc_w(pc, offset, false);

// (+4,+1)
#define INSTR_ldc_w(pc,offset)                        \
  INSTR_ldc_or_ldc_w(pc, offset, true);

// (+4,+2)
#define INSTR_ldc2_w(pc,offset)                       \
  u2 index = Bytes::get_Java_u2((pc)+2);              \
                                                      \
  ConstantPool* constants = METHOD->constants();      \
  switch (constants->tag_at(index).value()) {         \
                                                      \
  case JVM_CONSTANT_Long:                             \
     SET_STACK_LONG(constants->long_at(index), offset+1);\
    break;                                            \
                                                      \
  case JVM_CONSTANT_Double:                           \
     SET_STACK_DOUBLE(constants->double_at(index), offset+1);\
    break;                                            \
                                                      \
  case JVM_CONSTANT_Dynamic:                          \
    {                                                 \
      oop result = constants->resolved_references()->obj_at(index);\
      if (result == NULL) {                           \
        CALL_VM(InterpreterRuntime::resolve_ldc(THREAD, Bytecodes::_ldc2_w), handle_exception);\
        result = THREAD->vm_result();                 \
      }                                               \
      VERIFY_OOP(result);                             \
                                                      \
      jvalue value;                                   \
      BasicType type = java_lang_boxing_object::get_value(result, &value);\
      switch (type) {                                 \
      case T_DOUBLE: SET_STACK_DOUBLE(value.d, offset+1); break;\
      case T_LONG:   SET_STACK_LONG(value.j, offset+1); break;\
      default:  ShouldNotReachHere();                 \
      }                                               \
                                                      \
      break;                                          \
    }                                                 \
  }



// (+3,+1) for non-wide, (+4,1) for wide
#define INSTR_fast_aldc_or_fast_aldc_w(pc,offset,is_wide)\
  u2 index;                                           \
  if (is_wide) {                                      \
    index = Bytes::get_native_u2((pc)+2);             \
  } else {                                            \
    index = (pc)[2];                                  \
  }                                                   \
                                                      \
  ConstantPool* constants = METHOD->constants();      \
  oop result = constants->resolved_references()->obj_at(index);\
  if (result == NULL) {                               \
    CALL_VM(InterpreterRuntime::resolve_ldc(THREAD,   \
            is_wide ? Bytecodes::_fast_aldc_w : Bytecodes::_fast_aldc),\
            handle_exception);                        \
    result = THREAD->vm_result();                     \
  }                                                   \
  if (oopDesc::equals(result, Universe::the_null_sentinel()))\
    result = NULL;                                    \
                                                      \
  VERIFY_OOP(result);                                 \
  SET_STACK_OBJECT(result, offset);


// (+3,+1)
#define INSTR_fast_aldc(pc,offset)                    \
  INSTR_fast_aldc_or_fast_aldc_w(pc, offset, false);

// (+4,+1)
#define INSTR_fast_aldc_w(pc,offset)                  \
  INSTR_fast_aldc_or_fast_aldc_w(pc, offset, true);

// (+?,+?) invoke
#define INSTR_invokedynamic(pc,offset)                \
  u4 index = Bytes::get_native_u4((pc)+2);            \
  ConstantPoolCacheEntry* cache = cp->constant_pool()->invokedynamic_cp_cache_entry_at(index);\
  if (! cache->is_resolved(Bytecodes::_invokedynamic)) {\
    CALL_VM(InterpreterRuntime::resolve_from_cache(THREAD, Bytecodes::_invokedynamic),\
            handle_exception);                        \
    cache = cp->constant_pool()->invokedynamic_cp_cache_entry_at(index);\
  }                                                   \
                                                      \
  Method* method = cache->f1_as_method();             \
  if (VerifyOops) method->verify();                   \
                                                      \
  if (cache->has_appendix()) {                        \
    ConstantPool* constants = METHOD->constants();    \
    SET_STACK_OBJECT(cache->appendix_if_resolved(constants), offset);\
    MORE_STACK(1);                                    \
  }                                                   \
                                                      \
  istate->set_msg(call_method);                       \
  istate->set_callee(method);                         \
  istate->set_callee_entry_point(method->from_interpreted_entry());\
  istate->set_bcp_advance(6);                         \
                                                      \
  BI_PROFILE_UPDATE_CALL();                           \
                                                      \
  UPDATE_PC_AND_RETURN(0);

// (+?,+?) invoke
#define INSTR_invokehandle(pc,offset)                 \
  u2 index = Bytes::get_native_u2((pc)+2);            \
  ConstantPoolCacheEntry* cache = cp->entry_at(index);\
                                                      \
  if (! cache->is_resolved(Bytecodes::_invokehandle)) {\
    CALL_VM(InterpreterRuntime::resolve_from_cache(THREAD, Bytecodes::_invokehandle),\
            handle_exception);                        \
    cache = cp->entry_at(index);                      \
  }                                                   \
                                                      \
  Method* method = cache->f1_as_method();             \
  if (VerifyOops) method->verify();                   \
                                                      \
  if (cache->has_appendix()) {                        \
    ConstantPool* constants = METHOD->constants();    \
    SET_STACK_OBJECT(cache->appendix_if_resolved(constants), offset);\
    MORE_STACK(1);                                    \
  }                                                   \
                                                      \
  istate->set_msg(call_method);                       \
  istate->set_callee(method);                         \
  istate->set_callee_entry_point(method->from_interpreted_entry());\
  istate->set_bcp_advance(4); /* TODO: encode pc offset */\
                                                      \
  BI_PROFILE_UPDATE_FINALCALL();                      \
                                                      \
  UPDATE_PC_AND_RETURN(0); // I'll be back...

// (+?,+?) invoke
#define INSTR_invokeinterface(pc,offset)              \
  u2 index = Bytes::get_native_u2((pc)+2);            \
  ConstantPoolCacheEntry* cache = cp->entry_at(index);\
  if (!cache->is_resolved(Bytecodes::_invokeinterface)) {\
    CALL_VM(InterpreterRuntime::resolve_from_cache(THREAD, Bytecodes::_invokeinterface),\
            handle_exception);                        \
    cache = cp->entry_at(index);                      \
  }                                                   \
                                                      \
  istate->set_msg(call_method);                       \
  Method* callee = NULL;                              \
  if (cache->is_forced_virtual()) {                   \
    CHECK_NULL(STACK_OBJECT(-(cache->parameter_size())));\
    if (cache->is_vfinal()) {                         \
      callee = cache->f2_as_vfinal_method();          \
      BI_PROFILE_UPDATE_FINALCALL();                  \
    } else {                                          \
      int parms = cache->parameter_size();            \
      oop rcvr = STACK_OBJECT(offset-parms);          \
      VERIFY_OOP(rcvr);                               \
      Klass* rcvrKlass = rcvr->klass();               \
      callee = (Method*) rcvrKlass->method_at_vtable(cache->f2_as_index());\
      BI_PROFILE_UPDATE_VIRTUALCALL(rcvrKlass);       \
    }                                                 \
  } else if (cache->is_vfinal()) {                    \
    int parms = cache->parameter_size();              \
    oop rcvr = STACK_OBJECT(offset-parms);            \
    CHECK_NULL(rcvr);                                 \
    Klass* recv_klass = rcvr->klass();                \
    Klass* resolved_klass = cache->f1_as_klass();     \
    if (!recv_klass->is_subtype_of(resolved_klass)) { \
      ResourceMark rm(THREAD);                        \
      char buf[200];                                  \
      jio_snprintf(buf, sizeof(buf), "Class %s does not implement the requested interface %s",\
        recv_klass->external_name(),                  \
        resolved_klass->external_name());             \
      VM_JAVA_ERROR(vmSymbols::java_lang_IncompatibleClassChangeError(), buf, note_no_trap);\
    }                                                 \
    callee = cache->f2_as_vfinal_method();            \
  }                                                   \
  if (callee != NULL) {                               \
    istate->set_callee(callee);                       \
    istate->set_callee_entry_point(callee->from_interpreted_entry());\
    istate->set_bcp_advance(6);                       \
    UPDATE_PC_AND_RETURN(0); /* I'll be back... */    \
  }                                                   \
                                                      \
  Method *interface_method = cache->f2_as_interface_method();\
  InstanceKlass* iclass = interface_method->method_holder();\
                                                      \
  int parms = cache->parameter_size();                \
  oop rcvr = STACK_OBJECT(offset-parms);              \
  CHECK_NULL(rcvr);                                   \
  InstanceKlass* int2 = (InstanceKlass*) rcvr->klass();\
                                                      \
  {                                                   \
    Klass* refc = cache->f1_as_klass();               \
    itableOffsetEntry* scan;                          \
    for (scan = (itableOffsetEntry*) int2->start_of_itable();\
         scan->interface_klass() != NULL;             \
         scan++) {                                    \
      if (scan->interface_klass() == refc) {          \
        break;                                        \
      }                                               \
    }                                                 \
    if (scan->interface_klass() == NULL) {            \
      VM_JAVA_ERROR(vmSymbols::java_lang_IncompatibleClassChangeError(), "", note_no_trap);\
    }                                                 \
  }                                                   \
                                                      \
  itableOffsetEntry* ki = (itableOffsetEntry*) int2->start_of_itable();\
  int i;                                              \
  for ( i = 0 ; i < int2->itable_length() ; i++, ki++ ) {\
    if (ki->interface_klass() == iclass) break;       \
  }                                                   \
  if (i == int2->itable_length()) {                   \
    CALL_VM(InterpreterRuntime::throw_IncompatibleClassChangeErrorVerbose(THREAD, rcvr->klass(), iclass),\
            handle_exception);                        \
  }                                                   \
  int mindex = interface_method->itable_index();      \
                                                      \
  itableMethodEntry* im = ki->first_method_entry(rcvr->klass());\
  callee = im[mindex].method();                       \
  if (callee == NULL) {                               \
    CALL_VM(InterpreterRuntime::throw_AbstractMethodErrorVerbose(THREAD, rcvr->klass(), interface_method),\
            handle_exception);                        \
  }                                                   \
                                                      \
  BI_PROFILE_UPDATE_VIRTUALCALL(rcvr->klass());       \
                                                      \
  istate->set_callee(callee);                         \
  istate->set_callee_entry_point(callee->from_interpreted_entry());\
  istate->set_bcp_advance(6);                         \
  UPDATE_PC_AND_RETURN(0); // I'll be back...

// (+?,?) invoke
#define INSTR_invoke_virtual_special_static(pc,offset,opcode)\
  u2 index = Bytes::get_native_u2((pc)+2);            \
                                                      \
  ConstantPoolCacheEntry* cache = cp->entry_at(index);\
  if (!cache->is_resolved(opcode)) {                  \
    CALL_VM(InterpreterRuntime::resolve_from_cache(THREAD, opcode),\
            handle_exception);                        \
    cache = cp->entry_at(index);                      \
  }                                                   \
                                                      \
  istate->set_msg(call_method);                       \
  {                                                   \
    Method* callee;                                   \
    if (opcode == Bytecodes::_invokevirtual) {        \
      CHECK_NULL(STACK_OBJECT(offset-(cache->parameter_size())));\
      if (cache->is_vfinal()) {                       \
        callee = cache->f2_as_vfinal_method();        \
        BI_PROFILE_UPDATE_FINALCALL();                \
      } else {                                        \
        int parms = cache->parameter_size();          \
        oop rcvr = STACK_OBJECT(offset-parms);        \
        VERIFY_OOP(rcvr);                             \
        Klass* rcvrKlass = rcvr->klass();             \
        callee = (Method*) rcvrKlass->method_at_vtable(cache->f2_as_index());\
        BI_PROFILE_UPDATE_VIRTUALCALL(rcvrKlass);     \
      }                                               \
    } else {                                          \
      if (opcode == Bytecodes::_invokespecial) {      \
        CHECK_NULL(STACK_OBJECT(offset-(cache->parameter_size())));\
      }                                               \
      callee = cache->f1_as_method();                 \
                                                      \
      BI_PROFILE_UPDATE_CALL();                       \
    }                                                 \
                                                      \
    istate->set_callee(callee);                       \
    istate->set_callee_entry_point(callee->from_interpreted_entry());\
    istate->set_bcp_advance(4);                       \
    UPDATE_PC_AND_RETURN(0); /* I'll be back... */    \
  }

// (+?,?) invoke
#define INSTR_invokevirtual(pc,offset)                \
  INSTR_invoke_virtual_special_static(pc, offset, Bytecodes::_invokevirtual);

// (+?,?) invoke
#define INSTR_invokestatic(pc,offset)                 \
  INSTR_invoke_virtual_special_static(pc, offset, Bytecodes::_invokestatic);

// (+?,?) invoke
#define INSTR_invokespecial(pc,offset)                \
  INSTR_invoke_virtual_special_static(pc, offset, Bytecodes::_invokespecial);

// (+3,0)
#define INSTR_newarray(pc,offset)                     \
  BasicType atype = (BasicType) *((pc)+2);            \
  jint size = STACK_INT(offset-1);                    \
  CALL_VM(InterpreterRuntime::newarray(THREAD, atype, size),\
          handle_exception);                          \
  OrderAccess::storestore();                          \
  SET_STACK_OBJECT(THREAD->vm_result(), offset-1);    \
  THREAD->set_vm_result(NULL);

// (+?,?) terminal
#define INSTR_athrow(pc,offset)                       \
  oop except_oop = STACK_OBJECT(offset-1);            \
  CHECK_NULL(except_oop);                             \
  THREAD->set_pending_exception(except_oop, NULL, 0); \
  goto handle_exception;

// (+?,?) jump
#define INSTR_jsr(pc,offset)                          \
  /* push bytecode index on stack */                  \
  SET_STACK_ADDR(((address)(pc) - (intptr_t)(istate->method()->code_base()) + 4), offset);\
  int16_t pc_offset = (int16_t)Bytes::get_Java_u2((pc) + 2);\
  BI_PROFILE_UPDATE_JUMP();                           \
  PROFILE_CONDITIONAL(WITH_PROFILE, (pc)+4);          \
  address branch_pc = pc;                             \
  SET_PC_AND_TOS(pc + pc_offset, offset + 1);         \
  DO_BACKEDGE_CHECKS(pc_offset, branch_pc);           \
  CONTINUE;

// (+?,?) jump
#define INSTR_goto(pc,offset)                         \
  int16_t pc_offset = (int16_t)Bytes::get_Java_u2((pc) + 2);\
  BI_PROFILE_UPDATE_JUMP();                           \
  PROFILE_CONDITIONAL(WITH_PROFILE, (pc)+4);          \
  address branch_pc = pc;                             \
  SET_PC_AND_TOS((pc) + pc_offset, offset);           \
  DO_BACKEDGE_CHECKS(pc_offset, branch_pc);           \
  CONTINUE;

// (+?,?) jump
#define INSTR_jsr_w(pc,offset)                        \
  /* push return address on the stack */              \
  SET_STACK_ADDR(((address)(pc) - (intptr_t)(istate->method()->code_base()) + 6), offset);\
  int32_t pc_offset = Bytes::get_Java_u4((pc) + 2);   \
  BI_PROFILE_UPDATE_JUMP();                           \
  PROFILE_CONDITIONAL(WITH_PROFILE, (pc)+6);          \
  address branch_pc = pc;                             \
  SET_PC_AND_TOS(pc + pc_offset, offset + 1);         \
  DO_BACKEDGE_CHECKS(pc_offset, branch_pc);           \
  CONTINUE;

// (+?,?) jump
#define INSTR_goto_w(pc,offset)                       \
  int32_t pc_offset = Bytes::get_Java_u4((pc) + 2);   \
  BI_PROFILE_UPDATE_JUMP();                           \
  PROFILE_CONDITIONAL(WITH_PROFILE, (pc)+6);          \
  address branch_pc = pc;                             \
  SET_PC_AND_TOS(pc + pc_offset, offset);             \
  DO_BACKEDGE_CHECKS(pc_offset, branch_pc);           \
  CONTINUE;

// (+?,?) jump
#define INSTR_ret(pc,offset)                          \
  PROFILE_CONDITIONAL(WITH_PROFILE, (pc)+2);          \
  BI_PROFILE_UPDATE_RET(/*bci=*/((int)(intptr_t)(LOCALS_ADDR((pc)[2]))));\
  SET_PC_AND_TOS(istate->method()->code_base() + (intptr_t)(LOCALS_ADDR((pc)[2])), offset);\
  CONTINUE;

// (+?,?) terminal
#define INSTR_breakpoint(pc,offset)                   \
  Bytecodes::Code original_bytecode;                  \
  DECACHE_STATE();                                    \
  SET_LAST_JAVA_FRAME();                              \
  original_bytecode = InterpreterRuntime::get_original_bytecode_at(THREAD,\
                      METHOD, pc);                    \
  RESET_LAST_JAVA_FRAME();                            \
  CACHE_STATE();                                      \
  if (THREAD->has_pending_exception()) goto handle_exception;\
    CALL_VM(InterpreterRuntime::_breakpoint(THREAD, METHOD, pc),\
                                            handle_exception);\
                                                      \
  opcode = (jubyte)original_bytecode;                 \
  goto opcode_switch;

// (+6,0) profile
#define INSTR_profile(pc, offset)                     \
  PROFILE_UNCONDITIONAL(pc+2);

// (+?,?) invoke terminal
#define INSTR_unimplemented(opcode)                  \
  fatal("Unimplemented opcode %d = %s", opcode,       \
        Bytecodes::name((Bytecodes::Code)opcode));    \
  goto finish;






