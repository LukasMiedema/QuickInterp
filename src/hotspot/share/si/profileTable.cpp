#include "precompiled.hpp"
#include "si/profileTable.hpp"
#include "utilities/concurrentHashTable.inline.hpp"

ProfileTable ProfileTable::_the_table;

class ProfileDataTableLookup : public StackObj {
private:
  Thread* _thread;
  uint32_t _record_id;

public:
  ProfileDataTableLookup(Thread* thread, uint32_t record_id)
    : _thread(thread), _record_id(record_id) {
  }

  uintx get_hash(void) const {
    return _record_id;
  }

  bool equals(ProfileRecord** value, bool* is_dead) {
    return (*value)->record_id == _record_id;
  }
};

class ProfileDataTableCreate : public StackObj {
private:
  Thread* _thread;
  uint32_t _record_id;
  std::string* _location;
  ProfileRecord* _return;
public:
  ProfileDataTableCreate(Thread* thread, uint32_t record_id, std::string* location) :
    _thread(thread), _record_id(record_id), _location(location), _return(NULL) {}

  ProfileRecord* operator()() {
    vmassert(_location != nullptr, "expecting location if creation is invoked");
    return new ProfileRecord(_record_id, *_location);
  }

  void operator()(bool inserted, ProfileRecord** other) {
    _return = *other;
  }

  ProfileRecord* get_record() {
    return _return;
  }
};

class ProfileDataTableConfig : public ProfileTableHash::BaseConfig {
 private:
 public:
  static uintx get_hash(ProfileRecord** const& value, bool* is_dead) {
    return (*value)->record_id;
  }
};

ProfileRecord* ProfileTable::get_or_insert(uint32_t record_id, std::string* location) {
  Thread* thread = Thread::current();
  ProfileDataTableLookup lookup(thread, record_id);
  ProfileDataTableCreate create(thread, record_id, location);
  _profile_table.get_insert_lazy(thread, lookup, create, create);
  return create.get_record();
}
