#ifndef CODESTRETCHER_HPP
#define CODESTRETCHER_HPP

#include "memory/allocation.hpp"
#include "jvm.h"
#include "prims/jvmtiExport.hpp"
#include "jnif.hpp"


class CodeStretcher: public StackObj {
  jvmtiEnv* env;
  const char *name;
  jint data_len;
  unsigned char *data;
  std::string unique_key;

public:
  CodeStretcher(jvmtiEnv* env, const char *name, jint data_len, unsigned char *data);

  /**
   * Perform a rewriting of the class (codestretching) implemented in native code with the
   * JNIF library. This performs the minimal amount of rewriting required to load the class
   * in the modified VM, but performs no superinstruction substitution.
   */
  void rewrite_class_native(bool enable_profiling);

  /**
   * Use the managed JVM SI service provider to perform superinstruction substitution.
   */
  void rewrite_class_java(JNIEnv *jni);

  /**
   * Mirror the state of the code stretcher to the given values. Note that this performs
   * no copying, it just sets the pointer of new_data.
   */
  void get_state(unsigned char **new_data, jint *new_data_len);

  /**
   * Load the SI version from cache if possible. Returns false if a class with this key
   * does not exist in the cache.
   */
  bool load_from_cache();

  /**
   * Writes the class to the filename.
   */
  void dump_to_file(std::string filename);

  /**
   * Get the unique identifier of this class
   */
  std::string get_unique_key();

private:
  /**
   * Compute a hash for the current class. Note that the hash changes
   * after transformations. The hash is not cryptographically secure.
   */
  uint64_t compute_hash();

  /**
   * Generates a filename-safe unique key for this class. It consists of the name (if present)
   * and the hash.
   */
  std::string compute_unique_key();

  /**
   * Insert a local execution counter profiling instruction before the given instruction.
   */
  void insert_lec_before(jnif::Inst *insn, jnif::Method &method);

  /**
   * Insert a local execution counter directly after a label.
   */
  void insert_lec_after_label(jnif::LabelInst *label, jnif::Method &method);
};

#endif
