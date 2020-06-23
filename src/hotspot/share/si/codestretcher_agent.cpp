#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <iostream>
#include <fstream>

#include "jvm.h"
#include "prims/jvmtiExport.hpp"
#include "jnif.hpp"
#include "codestretcher_agent.hpp"
#include "codestretcher.hpp"

using std::set;
using std::string;

static const set<string> boot_classlist {
  // si classes
  "jdk.internal.vm.si.ClassBytecodeFormatConverterProxy!3580459607571145728",

  // other
#include "bootclasslist.txt"
};

extern "C" JNICALL void class_file_load_hook(jvmtiEnv *jvmti, JNIEnv *jni,
    jclass class_being_redefined, jobject loader, const char *name,
    jobject protection_domain, jint data_len, const unsigned char *data,
    jint *new_data_len, unsigned char **new_data) {

  CodeStretcher stretcher(jvmti, name, data_len, (unsigned char*) data);
  stretcher.rewrite_class_native();

  std::string unique_key = stretcher.compute_unique_key();
  //std::cout << "Native rewriting " << unique_key << '\n';

  std::string target("testbench.Main!8890990170487007973");

  if (target == unique_key /*boot_classlist.find(unique_key) == boot_classlist.end()*/) {
    // Not on the boot list --> rewrite in java
    std::cout << "> JNI rewriting " << unique_key << '\n';
    stretcher.rewrite_class_java(jni);
    if (jni->ExceptionCheck() == JNI_TRUE)
      return;
  }

  stretcher.get_state(new_data, new_data_len);


//  if (name != NULL && strcmp("java/util/concurrent/ConcurrentHashMap", name) == 0) {
//      // To file
//      printf("Exporting class %s to Output.class\n", name);
//      stretcher.dump_to_file("Output.class");
//  }
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
