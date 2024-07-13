#include "hrnode.hpp"

#include <stdexcept>

namespace hotring {

/*
 * ITEM
 */
ItemNode* ItemNode::get_next() {
  auto ptr = reinterpret_cast<size_t>(_next);
  ptr &= ADDRESS_MASK;
  return reinterpret_cast<ItemNode*>(ptr);
}

// rehash
bool ItemNode::is_rehash() {
  auto ptr = reinterpret_cast<size_t>(_next);
  return (bool)(ptr & REHASH_MASK);
}

void ItemNode::set_rehash() {
  auto ptr = reinterpret_cast<size_t>(_next);
  ptr |= REHASH_MASK;
  _next = reinterpret_cast<ItemNode*>(ptr);
}

void ItemNode::reset_rehash() {
  auto ptr = reinterpret_cast<size_t>(_next);
  ptr &= ~REHASH_MASK;
  _next = reinterpret_cast<ItemNode*>(ptr);
}

// occupy
bool ItemNode::is_occupied() {
  auto ptr = reinterpret_cast<size_t>(_next);
  return (bool)(ptr & OCCUPIED_MASK);
}

void ItemNode::set_occupied() {
  auto ptr = reinterpret_cast<size_t>(_next);
  ptr |= OCCUPIED_MASK;
  _next = reinterpret_cast<ItemNode*>(ptr);
}

void ItemNode::reset_occupied() {
  auto ptr = reinterpret_cast<size_t>(_next);
  ptr &= ~OCCUPIED_MASK;
  _next = reinterpret_cast<ItemNode*>(ptr);
}

// count
size_t ItemNode::get_count() {
  auto ptr = reinterpret_cast<size_t>(_next);
  ptr <<= 2;
  return ptr >>= 50;
}

void ItemNode::inc_count() {
  if (get_count() + 1 == MAX_COUNT) {
    throw std::runtime_error("The counter is out of range");
  }

  auto ptr = reinterpret_cast<size_t>(_next);
  ptr += BASIC_INC;
  _next = reinterpret_cast<ItemNode*>(ptr);
}

void ItemNode::reset_count() {
  auto ptr = reinterpret_cast<size_t>(_next);
  // 1100 0000 0000 0000 1111 ...
  ptr &= 0xC000FFFFFFFFFFFF;
  _next = reinterpret_cast<ItemNode*>(ptr);
}

/*
 * HEAD
 */
ItemNode* HeadNode::get_head() {
  auto ptr = reinterpret_cast<size_t>(_head);
  ptr &= ADDRESS_MASK;
  return reinterpret_cast<ItemNode*>(ptr);
}

void HeadNode::set_head(ItemNode* ptr) { _head = ptr; }

// active
bool HeadNode::is_active() {
  auto ptr = reinterpret_cast<size_t>(_head);
  return (bool)(ptr & ACTIVE_MASK);
}

void HeadNode::set_active() {
  auto ptr = reinterpret_cast<size_t>(_head);
  ptr |= ACTIVE_MASK;
  _head = reinterpret_cast<ItemNode*>(ptr);
}

void HeadNode::reset_active() {
  auto ptr = reinterpret_cast<size_t>(_head);
  ptr &= ~ACTIVE_MASK;
  _head = reinterpret_cast<ItemNode*>(ptr);
}

// count
size_t HeadNode::get_total_count() {
  auto ptr = reinterpret_cast<size_t>(_head);
  ptr <<= 1;
  return ptr >>= 49;
}

void HeadNode::inc_total_count() {
  if (get_total_count() + 1 == MAX_COUNT) {
    throw std::runtime_error("The counter is out of range");
  }

  auto ptr = reinterpret_cast<size_t>(_head);
  ptr += BASIC_INC;
  _head = reinterpret_cast<ItemNode*>(ptr);
}

void HeadNode::reset_total_count() {
  auto ptr = reinterpret_cast<size_t>(_head);
  // 1000 0000 0000 0000 1111 ...
  ptr &= 0x8000FFFFFFFFFFFF;
  _head = reinterpret_cast<ItemNode*>(ptr);
}

void HeadNode::adjust_tag(size_t split) {
  ItemNode *cur = get_head(), *head = cur;
  do {
    cur->set_tag(cur->get_tag() - split);
    cur = cur->get_next();
  } while (cur != head);
}

}  // namespace hotring
