#include <gtest/gtest.h>

#include "hotring.hpp"

TEST(HotRingTest, Read) {
  hotring::HotRing ring(16);

  // read null
  EXPECT_EQ(ring.read("null").first, false);
}
