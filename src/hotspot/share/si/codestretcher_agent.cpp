#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <iostream>
#include <fstream>

#include "jvm.h"
#include "prims/jvmtiExport.hpp"
#include "jnif.hpp"
#include "codestretcher_agent.hpp"

extern "C" JNICALL void class_file_load_hook(jvmtiEnv* jvmti, JNIEnv* jni,
                               jclass class_being_redefined,
							   jobject loader,
                               const char* name,
							   jobject protection_domain,
                               jint data_len,
							   const unsigned char* data,
                               jint* new_data_len,
							   unsigned char** new_data) {

//    if (strcmp("java/lang/Object", name) == 0)
//    	return;
//    printf("Rewriting class %s, length %i\n", name, data_len);

    try {

        // Read class
        jnif::parser::ClassFileParser cf(data, data_len);


        // Rewrite methods
        for (jnif::Method &method : cf.methods) {
            if (method.hasCode()) {

                // Remove attributes which break when the bytecode is stretched
                // Currently applies to LocalVariableTable and LineNumberTable attributes (makes debugging harder!)
                jnif::CodeAttr* code = method.codeAttr();
                std::vector<jnif::Attr*>* attrs = &(code->attrs.attrs);
                for (auto it = attrs->begin(); it != attrs->end(); it++) {
                    if ((*it)->kind == jnif::AttrKind::ATTR_LVT || (*it)->kind == jnif::AttrKind::ATTR_LNT) {
                        attrs->erase(it--);
                    }
                }


                jnif::InstList& instList = method.instList();
                //jnif::model::InstList::Iterator it = instList.begin();
                //++it;
                //for (; it != instList.end(); ++it) {
//                if (name != NULL && strcmp("java/lang/Object", name) == 0 && strcmp("finalize", method.getName()) == 0)
//                        continue;

                for (auto it = instList.begin(); it != instList.end(); ++it) {
    //				std::cout << "+nop " << name << " " << method.getName() << ":" << it->_offset << ", instr " << bytecode_names[(int) it->opcode] << ", kind " << kind_names[it->kind] << "\n";
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
        *new_data_len = cf.computeSize();
        jvmti->Allocate(*new_data_len, new_data);
        cf.write(*new_data, *new_data_len);

//        if (name != NULL && strcmp("java/time/Duration", name) == 0) {
//            // To file
//            printf("Exporting class %s to Output.class\n", name);
//            std::ofstream fout;
//            fout.open("Output.class", std::ios::binary | std::ios::out);
//            fout.write((char*) *new_data, *new_data_len);
//            fout.close();
//        }

    } catch (jnif::Exception e) {
        std::cerr << "Exception: " << e.message << "\n";
        std::cerr << "Exception: " << e.stackTrace << "\n";

        // To file
        printf("Exporting class %s to Error.class\n", name);
        std::ofstream fout;
        fout.open("Error.class", std::ios::binary | std::ios::out);
        fout.write((char*) data, data_len);
        fout.close();

        exit(-1);
    }
}


extern "C" jint JNICALL Agent_OnLoad_codestretcher(JavaVM *jvm, char *options, void *reserved)
{
  jvmtiEnv *jvmti = NULL;
  jint result = jvm->GetEnv((void **) &jvmti, JVMTI_VERSION_11);
  if (result != JNI_OK) {
 	  std::cerr << "ERROR: Unable to access JVMTI!\n";
  }

  jvmtiCapabilities capa = {0};

  capa.can_generate_all_class_hook_events = 1;
  capa.can_generate_early_class_hook_events = 1;
  (jvmti)->AddCapabilities(&capa);

  jvmtiEventCallbacks callbacks = {0};
  callbacks.ClassFileLoadHook = class_file_load_hook;
  jvmti->SetEventCallbacks(&callbacks, sizeof(callbacks));

  jvmti->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_CLASS_FILE_LOAD_HOOK, NULL);
  return JNI_OK;
}
