#ifndef INCLUDE_INCLUDE_UTILS_HPP_
#define INCLUDE_INCLUDE_UTILS_HPP_

#include <random>
#include <string>

static std::string gen_random_string(size_t bytes) {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  static std::uniform_int_distribution<> dis(
      33, 126);  // printable char excluding space

  std::string result(bytes, 0);
  for (size_t i = 0; i < bytes; ++i) {
    result[i] = static_cast<char>(dis(gen));
  }
  return result;
}

class DataSet {
  using kvpair = std::pair<std::string, std::string>;

 public:
  DataSet(size_t total_size) : _total_size(total_size) {
    while (_keys.size() < total_size) {
      std::string key = gen_random_string(8);
      std::string value = gen_random_string(16);
      _keys.emplace_back(key, value);
    }
  }

  void transform(double hot_ratio) {
    size_t hot_size = _total_size * hot_ratio;
    while (_hot_keys.size() != hot_size) {
      if (_hot_keys.size() > hot_size) {
        auto item = _hot_keys.back();
        _keys.push_back(item);
        _hot_keys.pop_back();
      } else {
        auto item = _keys.back();
        _hot_keys.push_back(item);
        _keys.pop_back();
      }
    }
  };

  kvpair get() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::bernoulli_distribution hot_spot_dist(
        0.8);  // generate one/zero with possibility of 0.8/0.2
    std::uniform_int_distribution<> dis(0, _keys.size() - 1);
    std::uniform_int_distribution<> hot_dis(0, _hot_keys.size() - 1);

    if (_hot_keys.empty()) {
      return _keys[dis(gen)];
    }

    if (_keys.empty()) {
      return _hot_keys[hot_dis(gen)];
    }

    return hot_spot_dist(gen) ? _keys[dis(gen)] : _hot_keys[hot_dis(gen)];
  }

 public:  // NOLINT: set public for test
  size_t _total_size;
  std::vector<kvpair> _keys;
  std::vector<kvpair> _hot_keys;
};

#endif  // INCLUDE_INCLUDE_UTILS_HPP_
