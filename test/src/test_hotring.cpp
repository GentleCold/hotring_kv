#include <gtest/gtest.h>

#include <unordered_map>

#include "hotring.hpp"
#include "utils.hpp"

TEST(HotRingTest, Put) {
  hotring::HotRing ring(2);  // 2^2 = 4

  // simple put and read
  ring.put("a", "b");
  auto ret = ring.read("a");
  EXPECT_EQ(ret.first, true);
  EXPECT_EQ(ret.second, "b");

  // update
  ring.put("a", "c");
  ret = ring.read("a");
  EXPECT_EQ(ret.first, true);
  EXPECT_EQ(ret.second, "c");

  // random put and read
  std::unordered_map<std::string, std::string> kv;
  for (int i = 0; i <= 100; ++i) {
    auto key = gen_random_string(8);
    auto value = gen_random_string(16);
    ring.put(key, value);
    kv[key] = value;
  }

  for (const auto& item : kv) {
    EXPECT_EQ(ring.read(item.first).second, item.second);
  }
}

TEST(HotRingTest, Read) {
  hotring::HotRing ring(8);

  // read null
  EXPECT_EQ(ring.read("null").first, false);

  ring.put("b", "");
  ring.put("d", "");
  ring.put("f", "");
  EXPECT_EQ(ring.read("a").first, false);
  EXPECT_EQ(ring.read("c").first, false);
  EXPECT_EQ(ring.read("e").first, false);
  EXPECT_EQ(ring.read("g").first, false);

  EXPECT_EQ(ring.read("b").first, true);
  EXPECT_EQ(ring.read("d").first, true);
  EXPECT_EQ(ring.read("f").first, true);
}
