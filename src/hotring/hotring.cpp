#include "hotring.hpp"

#include <iostream>
#include <limits>
#include <memory>

namespace hotring {

void HotRing::put(const std::string& key, const std::string& value) {
  size_t hash = _hash_func(key);
  // index bits + tag bits
  size_t index = (hash & (~_tag_mask)) >> _tag_bits;
  size_t tag = hash & _tag_mask;

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
  ++_total_read;  // per request
  ++_total_access;
  size_t hash = _hash_func(key);
  // index bits + tag bits
  size_t index = (hash & (~_tag_mask)) >> _tag_bits;
  size_t tag = hash & _tag_mask;

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

      if (!active && _total_read % HOTSPOT_R == 0 && head->get_head() != cur) {
        // find, but not the head node, start sampling
        head->set_active();
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
    ++_total_access;  // access for next item
  }

  if (active) {
    head->inc_total_count();
    if (head->get_total_count() >= _sample_num) {  // move head to hotspot
      _move_head(head, cur);
    }
  } else if (_get_average_load() >= MAX_LOAD_FACTOR) {
    _rehash();
    // reset average load
    _total_read = 0;
    _total_access = 0;
  }

  return {find, ret};
}

void HotRing::_move_head(HeadNode* head, ItemNode* cur) {
  size_t size = head->get_size();
  ItemNode* new_head = nullptr;

  // calculate income of cur node
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

  // find min income
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
}

// rehash the hash table, not consider multi threads
void HotRing::_rehash() {
  std::cout << "rehash\n";
  // double enlarge the hash table
  std::vector<std::unique_ptr<HeadNode>> new_table;
  size_t old_size = _table.size();
  new_table.resize(_table.size() << 1);

  --_tag_bits;
  assert(_tag_bits > 0);
  _tag_mask = (1 << _tag_bits) - 1;

  // x -> 2x + 1
  size_t split = 1 << _tag_bits;
  for (int i = 0; i < old_size; ++i) {
    if (_table[i] != nullptr) {
      auto* cur = _table[i]->get_head();
      bool first_bigger = false;
      size_t first_size = 0;
      size_t total_size = _table[i]->get_size();
      if (cur->get_tag() >= split) {
        first_bigger = true;
        new_table[(i << 2) + 1] = std::move(_table[i]);
      } else {
        new_table[(i << 2)] = std::move(_table[i]);
      }

      // 1 2 [3] 4 5
      // 1 [2]
      ItemNode* biggest_node = nullptr;
      ItemNode* split_node = nullptr;
      // find the two node
      while (true) {
        auto* next = cur->get_next();
        if (next == cur) {  // only one node
          break;
        }

        if (cur->get_tag() > next->get_tag()) {
          biggest_node = cur;
        } else if (cur->get_tag() < split && next->get_tag() >= split) {
          split_node = cur;
        }

        if (biggest_node && split_node) {
          break;
        }
        cur = next;
      }

      // split the node and set new head
      if (biggest_node) {
        auto* tmp = biggest_node->get_next();
        biggest_node->set_next(split_node->get_next());
        split_node->set_next(tmp);

        if (first_bigger) {
          new_table[i << 2] = std::make_unique<HeadNode>(split_node);
        } else {
          new_table[(i << 2) + 1] = std::make_unique<HeadNode>(biggest_node);
        }
      }
    }
  }
}

}  // namespace hotring
