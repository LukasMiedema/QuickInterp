#include "codestretcher.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>

using std::cerr;
using std::cout;

CodeStretcher::CodeStretcher(jvmtiEnv *env, const char *name, jint data_len, unsigned char *data) :
    env(env), name(name), data_len(data_len), data(data) {

}

void CodeStretcher::rewrite_class_native(void) {
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
