#include <algorithm>
#include <ctime>
#include <string>

std::string gen_random_string(size_t length) {
  std::srand(std::time(nullptr));
  std::string result(length, '\0');
  std::generate_n(result.begin(), length,
                  []() { return static_cast<char>(std::rand() % 256); });
  return result;
}
