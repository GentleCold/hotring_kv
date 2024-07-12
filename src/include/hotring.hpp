#ifndef INCLUDE_INCLUDE_HOTRING_HPP_
#define INCLUDE_INCLUDE_HOTRING_HPP_

#include <cassert>
#include <memory>
#include <vector>

#include "hrnode.hpp"

namespace hotring {

constexpr size_t HOTSPOT_R = 5;

class HotRing {
 public:
  HotRing(size_t bits) : _index_bits(bits), _table(0) {
    assert(bits < 64);
    size_t size = 1 << bits;
    _hash_mask = size - 1;
    _table.resize(size);
  };

  void put(const std::string& key, const std::string& value);
  std::pair<bool, std::string> read(const std::string& key);

  double get_average_load() const { return _total_access / _total_read; }

 private:
  void _move_head(HeadNode* head, ItemNode* cur);
  // n bits tag +  k bits hash
  inline static std::hash<std::string> _hash_func =
      std::hash<std::string>();  // hash func for string
  size_t _total_read = 0;        // inc every read
  size_t _total_access = 0;      // total node access times
  size_t _sample_num;
  size_t _index_bits;
  size_t _hash_mask;

  std::vector<std::unique_ptr<HeadNode>> _table;  // hash table
};

}  // namespace hotring

#endif  // INCLUDE_INCLUDE_HOTRING_HPP_
