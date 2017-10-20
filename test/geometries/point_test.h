#pragma once

#include "../test_helper.h"

#include "roulette/geometries/point.h"

using namespace roulette;

TEST(GeometryPointTest, point_test) {
  RandomGenerator generator;
  geometries::Point point({1, 2.5, -10.1});

  auto three_vector = point.sample(generator);
  EXPECT_EQ(three_vector(0), 1.0);
  EXPECT_EQ(three_vector(1), 2.5);
  EXPECT_EQ(three_vector(2), -10.1);
}
