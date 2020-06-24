#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <iostream>
#include <fstream>

#include "jvm.h"
#include "prims/jvmtiExport.hpp"
#include "jnif.hpp"
#include "runtime/globals.hpp"
#include "codestretcher_agent.hpp"
#include "profiler.hpp"
#include "codestretcher.hpp"

using std::set;
using std::string;

static const set<string> boot_classlist {
  "sun.util.resources.cldr.provider.CLDRLocaleDataMetaInfo!18198788711634137309",
};

extern "C" JNICALL void class_file_load_hook(jvmtiEnv *jvmti, JNIEnv *jni,
    jclass class_being_redefined, jobject loader, const char *name,
    jobject protection_domain, jint data_len, const unsigned char *data,
    jint *new_data_len, unsigned char **new_data) {

  // Use the codestretcher + optional conversion
  CodeStretcher stretcher(jvmti, name, data_len, (unsigned char*) data);

  bool loaded_from_cache = false;

  // Try the cache first
  if (EnableProfiler || !(loaded_from_cache = stretcher.load_from_cache())) {
    stretcher.rewrite_class_native(EnableProfiler);
    std::string unique_key = stretcher.get_unique_key();

    stretcher.dump_to_file("stage-1-dump/" + unique_key);

    bool has_loader = loader != NULL;
    bool is_lambda = name == NULL;
    bool is_internal = !is_lambda && (std::string(name).rfind("jdk/internal", 0) == 0);
    bool is_bootlisted = boot_classlist.find(unique_key) != boot_classlist.end();

    if (UseSuperinstructions && has_loader && !is_internal && !is_bootlisted) {
      // Not on the boot list and not a core classes lambda --> rewrite in java
      stretcher.rewrite_class_java(jni);
      if (jni->ExceptionCheck() == JNI_TRUE)
        return;
    }
    stretcher.dump_to_file("stage-3-dump/" + unique_key);
    //std::cout << "Loaded " << stretcher.get_unique_key() << " just in time\n";
  }

  if (loaded_from_cache) {
    // Load from cache was successful
    //std::cout << "Loaded " << stretcher.get_unique_key() << " from cache\n";
  }

  stretcher.get_state(new_data, new_data_len);
}

extern "C" JNICALL void vm_death_hook(jvmtiEnv *jvmti, JNIEnv *jni) {
  if (EnableProfiler) {
    Profiler::on_shutdown();
  }
}


extern "C" jint JNICALL Agent_OnLoad_codestretcher(JavaVM *jvm, char *options, void *reserved)
{
  jvmtiEnv *jvmti = NULL;
  jint result = jvm->GetEnv((void **) &jvmti, JVMTI_VERSION_11);
  if (result != JNI_OK) {
 	  std::cerr << "ERROR: Unable to access JVMTI!\n";
 	  return JNI_ERR;
  }

  jvmtiCapabilities capa = {0};

  capa.can_generate_all_class_hook_events = 1;
  capa.can_generate_early_class_hook_events = 1;
  (jvmti)->AddCapabilities(&capa);

  jvmtiEventCallbacks callbacks = {0};
  callbacks.ClassFileLoadHook = class_file_load_hook;
  callbacks.VMDeath = vm_death_hook;
  jvmti->SetEventCallbacks(&callbacks, sizeof(callbacks));
  jvmti->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_CLASS_FILE_LOAD_HOOK, NULL);
  jvmti->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_VM_DEATH, NULL);

  if (UseSuperinstructions && EnableProfiler) {
    std::cerr << "ERROR: Superinstructions cannot be used when the profiler is enabled\n";
    return JNI_ERR;
  }

  if (UseSuperinstructions)
    std::cout << "Superinstructions enabled\n";
  if (EnableProfiler)
    std::cout << "Profiler enabled\n";

  return JNI_OK;
}
