#include <gtest/gtest.h>

#include "utils.hpp"

TEST(UtilTest, RandomString) {
  for (int i = 0; i < 50000; i++) {
    auto str = gen_random_string(16);
    EXPECT_EQ(str.length(), 16);
  }
}

TEST(UtilTest, DataSet) {
  // init
  DataSet data(10000);

  // transform
  data.transform(0.1);
  EXPECT_EQ(data._hot_keys.size(), 1000);
  EXPECT_EQ(data._keys.size(), 9000);

  data.transform(0.9);
  EXPECT_EQ(data._hot_keys.size(), 9000);
  EXPECT_EQ(data._keys.size(), 1000);

  data.transform(0.5);
  EXPECT_EQ(data._hot_keys.size(), 5000);
  EXPECT_EQ(data._keys.size(), 5000);

  data.transform(1.0);
  EXPECT_EQ(data._hot_keys.size(), 10000);
  EXPECT_EQ(data._keys.size(), 0);

  // random get
  for (int i = 0; i < 10000; i++) {
    auto item = data.get();
    EXPECT_EQ(item.first.size(), 8);
    EXPECT_EQ(item.second.size(), 16);
  }
  data.transform(0.5);
  for (int i = 0; i < 10000; i++) {
    auto item = data.get();
    EXPECT_EQ(item.first.size(), 8);
    EXPECT_EQ(item.second.size(), 16);
  }
}
