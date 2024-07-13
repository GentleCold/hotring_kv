#include <gtest/gtest.h>

#include "hrnode.hpp"

TEST(NodeTest, Counter) {
  // init
  auto *item = new hotring::ItemNode(0, nullptr, "key", "value");
  item->set_next(item);
  auto head = hotring::HeadNode(item);

  // increase 10 times
  EXPECT_EQ(head.get_total_count(), 0U);
  for (int i = 0; i < 10; ++i) {
    head.inc_total_count();
  }

  EXPECT_EQ(head.get_total_count(), 10U);
  head.reset_total_count();
  EXPECT_EQ(head.get_total_count(), 0U);

  // increase again
  for (int i = 0; i < 10; ++i) {
    head.inc_total_count();
  }

  EXPECT_EQ(head.get_total_count(), 10U);
  head.reset_total_count();

  // item test
  EXPECT_EQ(item->get_count(), 0U);
  for (int i = 0; i < 10; ++i) {
    item->inc_count();
  }

  EXPECT_EQ(item->get_count(), 10U);
  item->reset_count();
  EXPECT_EQ(item->get_count(), 0U);
}

TEST(NodeTest, Flag) {
  // init
  auto *item = new hotring::ItemNode(0, nullptr, "key", "value");
  item->set_next(item);
  auto head = hotring::HeadNode(item);

  // active
  EXPECT_EQ(head.is_active(), false);
  head.set_active();
  EXPECT_EQ(head.is_active(), true);
  head.reset_active();
  EXPECT_EQ(head.is_active(), false);

  // rehash
  EXPECT_EQ(item->is_rehash(), false);
  item->set_rehash();
  EXPECT_EQ(item->is_rehash(), true);
  item->reset_rehash();
  EXPECT_EQ(item->is_rehash(), false);
}

TEST(NodeTest, SafeRelease) {
  // init
  auto *item = new hotring::ItemNode(0, nullptr, "key", "value");
  item->set_next(item);
  auto *head = new hotring::HeadNode(item);

  head->inc_total_count();
  head->set_active();

  item->inc_count();
  item->set_rehash();
  item->set_occupied();

  // should be safe release
  delete head;
}
