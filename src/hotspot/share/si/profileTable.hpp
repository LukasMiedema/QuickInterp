/*
 * profiler.hpp
 *
 *  Created on: Feb 6, 2020
 *      Author: lukas
 */

#ifndef SRC_HOTSPOT_SHARE_SI_PROFILE_TABLE_HPP_
#define SRC_HOTSPOT_SHARE_SI_PROFILE_TABLE_HPP_

#include <string>
#include "memory/allocation.hpp"

#include "utilities/concurrentHashTable.hpp"

class ProfileRecord;
class ProfileDataTableConfig;
typedef ConcurrentHashTable<ProfileRecord*, ProfileDataTableConfig, mtInternal> ProfileTableHash;

class ProfileRecord {
public:
  const uint32_t record_id;
  volatile uint64_t count;
  std::string location;
  ProfileRecord(uint32_t record_id, std::string location) : record_id(record_id), count(0), location(location) {}
};

class ProfileTable : public CHeapObj<mtSymbol>{
private:
  ProfileTableHash _profile_table;
  static ProfileTable _the_table;

public:
  static ProfileTable* get_table() {
    return &_the_table;
  }

  ProfileRecord* get_or_insert(uint32_t record_id, std::string* location = nullptr);
};
#endif /* SRC_HOTSPOT_SHARE_SI_PROFILE_TABLE_HPP_ */
