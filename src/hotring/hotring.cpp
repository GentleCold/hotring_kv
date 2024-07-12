#include "hotring.hpp"

#include <limits>

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

std::pair<bool, std::string> HotRing::read(const std::string& key) {  // NOLINT
  ++_access;  // per request
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

  // is active
  bool active = head->is_active();

  while (true) {
    if (active) {
      cur->inc_count();
    }

    // compare tag first (tag, key)
    if ((*cur) == compare) {
      find = true;
      ret = cur->get_value();

      if (!active && _access >= HOTSPOT_R && head->get_head() != cur) {
        // find, but not the head node, start sampling
        head->set_active();
        _access = 0;
        _sample_num = head->get_size();
      }

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

  if (active) {
    head->inc_total_count();
    if (head->get_total_count() >= _sample_num) {  // move head to hotspot
      size_t size = head->get_size();
      ItemNode* new_head = nullptr;

      auto calculate = [size](ItemNode* cur) -> size_t {
        size_t income = 0;
        size_t i = 0;
        while (i < size) {
          income += i * cur->get_count();
          cur = cur->get_next();
          ++i;
        }
        return income;
      };

      size_t i = 0;
      size_t min_size = std::numeric_limits<size_t>::max();
      while (i < size) {
        if (calculate(cur) < min_size) {
          new_head = cur;
        }
        cur = cur->get_next();
        ++i;
      }

      i = 0;
      while (i < size) {
        cur->reset_count();
        cur = cur->get_next();
        ++i;
      }

      head->set_head(new_head);
      head->reset_active();
      head->reset_total_count();
      _access = 0;
    }
  }

  return {find, ret};
}

}  // namespace hotring
