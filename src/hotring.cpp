#include "hotring.hpp"

#include <iostream>

namespace hotring {

std::pair<bool, std::string> HotRing::read(const std::string& key) {
  size_t hash = _hash_func(key);
  size_t index = hash & _hash_mask;
  size_t tag = (hash & (~_hash_mask)) >> _index_bits;

  HeadNode* head = _table[index].get();
  if (head == nullptr) {
    return {false, ""};
  }

  ItemNode compare(tag, nullptr, key, "");
  ItemNode* cur = head->get_head();
  bool find = false;
  std::string ret;
  // while (cur) {
  //   // compare tag first (tag, key)
  //   if (*cur == compare) {
  //     find = true;
  //     ret = cur->get_value();
  //     break;
  //   }
  //
  //   cur = cur->get_next();
  // }

  return {find, ret};
}

}  // namespace hotring
