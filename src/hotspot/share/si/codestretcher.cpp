#include "codestretcher.hpp"
#include "profiler.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>

using std::cerr;
using std::cout;

CodeStretcher::CodeStretcher(jvmtiEnv *env, const char *name, jint data_len, unsigned char *data) :
    env(env), name(name), data_len(data_len), data(data) {
  unique_key = compute_unique_key();
}

void CodeStretcher::insert_lec_before(jnif::Inst *insn, jnif::Method &method) {
  jnif::InstList &instList = method.instList();
  auto nop1 = instList.addZero(jnif::Opcode::nop, insn);
  auto insert1 = instList.addProfile(Profiler::allocate_instruction(this->unique_key, method.getName(), method.getDesc(), insn->_offset, BranchPointType::LOCAL_EXECUTION_COUNTER), insn);

  // All get the bco of the instruction they replace
  nop1->_offset = insn->prev->_offset;
  insert1->_offset = insn->_offset;
}

void CodeStretcher::insert_lec_after_label(jnif::LabelInst *label, jnif::Method &method) {
  // Labels are a filthy hack with codestretching, they are placed -within- the codestretched instruction
  // Basically, it looks like this:
  //   <nop1>
  //   <label>
  //   <instr1>
  // And we make it look like this
  //   <nop1>
  //   <label>
  //   <instr2>
  //   <nop2>
  //   <instr1>

  // So to insert something after the label, we insert the new instruction and then a nop, not the other way around
  jnif::InstList &instList = method.instList();
  jnif::Inst* insn_nop = label->prev; // this is <nop1>, use this for the bco
  jnif::Inst* insn = label->next; // this is <instr1>

  // Insert <insert2> before
  auto insert2 = instList.addProfile(Profiler::allocate_instruction(this->unique_key, method.getName(), method.getDesc(), insn_nop->_offset, BranchPointType::LOCAL_EXECUTION_COUNTER), insn);

  // Insert <nop2>
  auto nop2 = instList.addZero(jnif::Opcode::nop, insn);

  // All get the bco of the previous instruction (nop1 or instr1)
  nop2->_offset = insn_nop->_offset;
  insert2->_offset = insn->_offset;
}

void CodeStretcher::rewrite_class_native(bool enable_profiling) {
  try {
    jnif::parser::ClassFileParser cf(this->data, this->data_len);
    for (jnif::Method &method : cf.methods) {
      if (method.hasCode()) {

        // Remove attributes which break when the bytecode is stretched
        // Currently applies to LocalVariableTable and LineNumberTable attributes (makes debugging harder!)
        jnif::CodeAttr *code = method.codeAttr();
        std::vector<jnif::Attr*> *attrs = &(code->attrs.attrs);
        for (auto it = attrs->begin(); it != attrs->end(); it++) {
          if ((*it)->kind == jnif::AttrKind::ATTR_LVT) {
            attrs->erase(it--);
          }
        }

        jnif::InstList &instList = method.instList();
        for (auto it = instList.begin(); it != instList.end(); ++it) {
          instList.addZero(jnif::Opcode::nop, *it);
          if (it->isLabel()) {
            // Skip next one, sandwiching the jump target between the nop and the actual instruction.
            // Jump targets on the JVM are relative, and they will originate from the _second_ part
            // of the code-stretched instruction (the first part is a nop) when JNIF writes the
            // class.
            // In other words, they are off by one byte. This is corrected by also offsetting the
            // labels by one byte, by putting them within the instruction.
            ++it;
          }
        }
      }
    }

    // All instructions are stretched
    // If profiling is enabled, and instr is a jump instruction, embed profiling data
    // See Profiler.hpp
    if (enable_profiling) {
      // By forcing a write with any writer (SizeWriter or whatever), the bco within the instruction
      // stream is set on every Insn instance

      this->data_len = cf.computeSize();
      this->env->Allocate(this->data_len, &this->data);
      cf.write(this->data, this->data_len);
      dump_to_file("class-dump/" + unique_key);
      this->env->Deallocate(this->data);

      for (jnif::Method &method : cf.methods) {
        if (method.hasCode()) {

          // Modify all instructions again
          jnif::InstList &instList = method.instList();
          auto it = instList.begin();
          insert_lec_before(*it, method);

          for (; it != instList.end(); ++it) {

            if (it->opcode == jnif::Opcode::nop) {
              jnif::Inst *insn_nop = *it;

              // Got the first part of an instruction. Find the second part
              for (++it; it->isLabel(); ++it);
              jnif::Inst *insn_main = *it;
              jnif::Inst *insn_next = (*it)->next; // a NOP, do next->next for the real instr

              if (insn_main->isJump()) {

                // Jumps are tracked directly
                u4 profile_id = Profiler::allocate_instruction(this->unique_key, method.getName(), method.getDesc(), insn_nop->_offset, BranchPointType::JUMP_COUNTER);
                insn_main->jump()->set_profile(profile_id);
              } else if (insn_main->isLookupSwitch() || insn_main->isTableSwitch()) {

                // Table/lookupswitch get a LEC at every branch
                jnif::SwitchInst *insn_switch = (jnif::SwitchInst*) insn_main;
                insert_lec_after_label(insn_switch->def->label(), method);

                for (std::vector<jnif::Inst*>::iterator target = insn_switch->targets.begin(); target != insn_switch->targets.end(); ++target) {
                  insert_lec_after_label((*target)->label(), method);
                }

              } else if (insn_main->isInvoke() || insn_main->isInvokeInterface() || insn_main->isInvokeDynamic()) {
                // Invokes get a LEC after the invoke, as the invoke may jump to an exception handler (which also have LECs)
                insert_lec_before(insn_next, method);
              }
            }
          }

          // Add labels for the exception handlers
          auto exceptions = &method.codeAttr()->exceptions;
          for (std::vector<jnif::CodeAttr::ExceptionHandler>::iterator handler = exceptions->begin(); handler != exceptions->end(); ++handler) {
            insert_lec_after_label((jnif::LabelInst*) handler->handlerpc, method);
          }
        }
      }
    }

    // Export class
    this->data_len = cf.computeSize();
    this->env->Allocate(this->data_len, &this->data);
    cf.write(this->data, this->data_len);

  } catch (jnif::Exception e) {
    cerr << "Exception: " << e.message << "\n";
    cerr << "Exception: " << e.stackTrace << "\n";
    cerr << "Exporting class " << name << " to Error.class\n";
    this->dump_to_file("Error.class");
    exit(-1);
  }
}

void CodeStretcher::rewrite_class_java(JNIEnv *jni) {
  // Do a JNI call to jdk/internal/vm/si/ClassBytecodeFormatConverterProxy#convertClass(byte[] b, int off, int len)
  // There will be some (highly redundant, unfortunately) copying around
  // 1. First copy is to a JNI byte array, so that the method can ingest it
  // 2. Second copy is from the JNI-returned byte array back to a "native" byte region so that the byte array can be released
  // Deeper integration with the VM could fix that, but eh

  jbyteArray input_buffer = jni->NewByteArray(this->data_len);
  jbyte* input_buffer_elems = jni->GetByteArrayElements(input_buffer, NULL);
  memcpy(input_buffer_elems, this->data, this->data_len);
  jni->ReleaseByteArrayElements(input_buffer, input_buffer_elems, 0);

  jclass proxy_class = jni->FindClass("jdk/internal/vm/si/ClassBytecodeFormatConverterProxy");
  if (jni->ExceptionCheck() == JNI_TRUE)
    return;

  jmethodID convert_class_method = jni->GetStaticMethodID(proxy_class, "convertClass", "([B)[B");
  if (jni->ExceptionCheck() == JNI_TRUE)
    return;

  jbyteArray output_buffer = (jbyteArray) jni->CallStaticObjectMethod(proxy_class, convert_class_method, input_buffer);
  if (jni->ExceptionCheck() == JNI_TRUE)
    return;

  jint output_buffer_len = jni->GetArrayLength(output_buffer);
  jbyte* output_buffer_elems = jni->GetByteArrayElements(output_buffer, NULL);
  if (jni->ExceptionCheck() == JNI_TRUE)
    return;

  // Assumption for this next line is that this is already-allocated data using env->Allocate
  this->env->Deallocate(this->data);
  this->env->Allocate(output_buffer_len, &this->data);
  memcpy(this->data, output_buffer_elems, output_buffer_len);
  this->data_len = output_buffer_len;

  jni->ReleaseByteArrayElements(output_buffer, output_buffer_elems, JNI_ABORT); // we only read
}

void CodeStretcher::get_state(unsigned char **new_data, jint *new_data_len) {
  *new_data = this->data;
  *new_data_len = this->data_len;
}

void CodeStretcher::dump_to_file(std::string filename) {
  std::ofstream fout;
  fout.open(filename, std::ios::binary | std::ios::out);
  fout.write((char*) this->data, this->data_len);
  fout.close();
}

std::string CodeStretcher::get_unique_key() {
  return this->unique_key;
}

uint64_t CodeStretcher::compute_hash(void) {
  uint64_t hash = 0x1234567890abcdef;
  hash ^= this->data_len;
  for (int i = 0; i < this->data_len; i++) {
    int offset = (i % (sizeof(uint64_t) / sizeof(jbyte))) * sizeof(jbyte) * 8;
    hash ^= ((uint64_t) this->data[i]) << offset;
  }
  return hash;
}

std::string CodeStretcher::compute_unique_key(void) {
  std::string str;
  if (this->name == NULL) {
    str = "<unnamed>";
  } else {
    str = std::string(this->name);
    for (uint i = 0; i < str.length(); i++) {
      char c = str[i];
      if ((c < 48 || c > 126) && c != '$')
        str[i] = '.';
    }
  }
  str += "!";
  str += std::to_string(this->compute_hash());
  return str;
}
