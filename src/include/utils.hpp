#include <iostream>
#include <random>
#include <string>

std::string gen_random_string(size_t length) {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  static std::uniform_int_distribution<> dis(0, 255);

  std::string result(length, 0);
  for (size_t i = 0; i < length; ++i) {
    result[i] = static_cast<char>(dis(gen));
  }
  return result;
}
