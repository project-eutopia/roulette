#pragma once

#include "../test_helper.h"

#include "roulette/geometries/rectangle.h"

using namespace roulette;

TEST(GeometryRectangleTest, rectangle_test) {
  RandomGenerator generator;
  geometries::Rectangle rectangle(
    {0, 0, 0},
    {2, 2, 0},
    {2, 2, 1}
  );

  for (int i = 0; i < 20; ++i) {
    auto three_vector = rectangle.sample(generator);

    EXPECT_GE(three_vector(0), 0);
    EXPECT_LT(three_vector(0), 2);
    EXPECT_GE(three_vector(1), 0);
    EXPECT_LT(three_vector(1), 2);
    EXPECT_GE(three_vector(2), 0);
    EXPECT_LT(three_vector(2), 1);

    EXPECT_EQ(three_vector(0), three_vector(1));
  }
}
