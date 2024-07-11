#ifndef INCLUDE_INCLUDE_HRNODE_HPP_
#define INCLUDE_INCLUDE_HRNODE_HPP_

#include <cassert>
#include <string>

namespace hotring {

constexpr size_t ADDRESS_MASK = (1UL << 48U) - 1;  // 2^48 - 1
constexpr size_t REHASH_MASK = 1UL << 63U;         // 2^63
constexpr size_t OCCUPIED_MASK = 1UL << 62U;       // 2^62
constexpr size_t BASIC_INC = 1UL << 48U;           // 2^48
constexpr size_t ACTIVE_MASK = REHASH_MASK;

class ItemNode {
 public:
  ItemNode(size_t tag, ItemNode* ptr, std::string key, std::string value)
      : _tag(tag), _next(ptr), _key(std::move(key)), _value(std::move(value)) {}

  size_t get_tag() const& { return _tag; }
  const std::string& get_key() const& { return _key; }
  const std::string& get_value() const& { return _value; }

  void set_next(ItemNode* ptr) { _next = ptr; }
  void set_value(const std::string& value) { _value = value; }

  ItemNode* get_next();

  // if rehash is true, then rehash is starting
  bool is_rehash();
  void set_rehash();
  void reset_rehash();

  // if occupied is true, then the node is being used
  bool is_occupied();
  void set_occupied();
  void reset_occupied();

  // get total visit times in current item
  size_t get_count();
  void inc_count();
  void reset_count();

  // for compare
  bool operator==(const ItemNode& other) const {
    return _tag == other._tag && _key == other._key;
  }

  bool operator>(const ItemNode& other) const {
    if (_tag == other._tag) {
      return _key > other._key;
    }
    return _tag > other._tag;
  }

 private:
  size_t _tag;
  // 1bit rehash + 1bit occupied + 14bits counter + 48bits address
  ItemNode* _next;
  std::string _key;
  std::string _value;

  static constexpr size_t MAX_COUNT = 1UL << 14U;  // 2^14
};

class HeadNode {
 public:
  explicit HeadNode(ItemNode* ptr) : _head(ptr) { assert(ptr != nullptr); }

  ItemNode* get_head();
  void set_head(ItemNode* ptr);

  void inc_size() { ++_size; }
  void dec_size() { --_size; }

  // if active is true, then make statistical sampling
  bool is_active();
  void set_active();
  void reset_active();

  // get total visit times in current ring
  size_t get_total_count();
  void inc_total_count();
  void reset_total_count();

  // safe release
  ~HeadNode() {
    ItemNode* cur = get_head();
    while (_size) {
      --_size;
      ItemNode* next = cur->get_next();
      delete cur;
      cur = next;
    }
  };

 private:
  size_t _size = 1;
  // 1bit active + 15bits counter + 48bits address
  ItemNode* _head;

  static constexpr size_t MAX_COUNT = 1UL << 15U;  // 2^15
};

}  // namespace hotring

#endif  // INCLUDE_INCLUDE_HRNODE_HPP_
