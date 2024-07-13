#ifndef INCLUDE_INCLUDE_HOTRING_HPP_
#define INCLUDE_INCLUDE_HOTRING_HPP_

#include <cassert>
#include <memory>
#include <vector>

#include "hrnode.hpp"

namespace hotring {

constexpr size_t HOTSPOT_R = 5;        // for hotspot
constexpr size_t MAX_LOAD_FACTOR = 2;  // for rehash

class HotRing {
 public:
  /**
   * @param bits - the num of bits of hash size (may rehash)
   */
  HotRing(size_t bits) : _tag_bits(64 - bits), _table(0) {
    assert(bits < 64);
    _tag_mask = (1UL << _tag_bits) - 1;
    _table.resize(1UL << bits);
  };

  void put(const std::string& key, const std::string& value);
  std::pair<bool, std::string> read(const std::string& key);

  size_t get_bucket_size() { return _table.size(); }

 private:
  double _get_average_load() const { return _total_access / _total_request; }
  void _move_head(HeadNode* head, ItemNode* cur);
  void _rehash();

  // k bits hash + n - k bits tag
  inline static std::hash<std::string> _hash_func =
      std::hash<std::string>();  // hash func for string
  size_t _total_request = 0;        // inc every read
  size_t _total_access = 0;      // total node access times
  size_t _sample_num;            // record sample num when active
  size_t _tag_bits;              // tag_bits may decrease
  size_t _tag_mask;

  std::vector<std::unique_ptr<HeadNode>> _table;  // hash table
};

}  // namespace hotring

#endif  // INCLUDE_INCLUDE_HOTRING_HPP_
