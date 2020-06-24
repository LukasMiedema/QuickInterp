#include "precompiled.hpp"
#include "si/profiler.hpp"
#include "si/profileTable.hpp"

#include <iostream>
#include <fstream>

volatile uint32_t Profiler::_last_id = 0;

void Profiler::on_shutdown(void) {
  std::ofstream fout;
  fout.open(PROFILE_FILENAME, std::ios::binary | std::ios::out);
  uint32_t last_id = Atomic::load(&_last_id);
  for (uint32_t id = 1; id <= last_id; id++) {
    ProfileRecord* record = ProfileTable::get_table()->get_or_insert(id);
    fout << record->location << " " << record->count << "\n";
  }
  fout.close();
  std::cout << "Profile written to " << PROFILE_FILENAME << "\n";
}

uint32_t Profiler::allocate_instruction(std::string class_id, std::string method_name, std::string method_desc, uint16_t bco, BranchPointType type) {
  uint32_t id = Atomic::add(uint32_t(1), &_last_id, memory_order_conservative);
  std::string type_name;
  switch (type) {
  case BranchPointType::JUMP_COUNTER:
    type_name = "jc";
    break;
  case BranchPointType::LOCAL_EXECUTION_COUNTER:
    type_name = "lec";
    break;
  }
  std::string location = type_name + " " + class_id + "!" + method_name + method_desc + ":" + std::to_string(bco);
  ProfileTable::get_table()->get_or_insert(id, &location);
  return id;
}

void Profiler::visit(uint32_t id) {
  ProfileRecord* record = ProfileTable::get_table()->get_or_insert(id);
  Atomic::inc(&(record->count), memory_order_conservative);
}
