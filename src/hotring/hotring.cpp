#include "hotring.hpp"

namespace hotring {

void HotRing::put(const std::string& key, const std::string& value) {
  size_t hash = _hash_func(key);
  size_t index = hash & _hash_mask;
  size_t tag = (hash & (~_hash_mask)) >> _index_bits;

  auto* item = new ItemNode(tag, nullptr, key, value);
  if (_table[index] == nullptr) {  // no item
    item->set_next(item);
    _table[index] = std::make_unique<HeadNode>(item);
    return;
  }

  HeadNode* head = _table[index].get();
  ItemNode* cur = head->get_head();

  while (true) {
    // update
    if ((*cur) == (*item)) {
      cur->set_value(value);
      delete item;
      break;
    }

    auto* next = cur->get_next();

    // between, largest or smallest
    // or only one item
    if ((cur == next) || (*cur > *next && (*item > *cur || *next > *item)) ||
        (*item > *cur && *next > *item)) {
      item->set_next(next);
      cur->set_next(item);
      head->inc_size();
      break;
    }

    cur = next;
  }
}

std::pair<bool, std::string> HotRing::read(const std::string& key) {
  size_t hash = _hash_func(key);
  size_t index = hash & _hash_mask;
  size_t tag = (hash & (~_hash_mask)) >> _index_bits;

  HeadNode* head = _table[index].get();
  if (head == nullptr) {
    return {false, ""};
  }

  ItemNode compare(tag, nullptr, key, "");
  ItemNode* cur = head->get_head();  // if head is not null, the cur exists
  bool find = false;
  std::string ret;

  while (true) {
    // compare tag first (tag, key)
    if ((*cur) == compare) {
      find = true;
      ret = cur->get_value();
      break;
    }

    auto* next = cur->get_next();
    if (cur == next) {  // only one item
      break;
    }

    // between, largest or smallest
    if ((*cur > *next && (compare > *cur || *next > compare)) ||
        (compare > *cur && *next > compare)) {
      break;
    }

    cur = next;
  }

  return {find, ret};
}

}  // namespace hotring
