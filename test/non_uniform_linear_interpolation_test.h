#pragma once

#include "test_helper.h"

#include "roulette/non_uniform_linear_interpolation.h"

TEST(NonUniformLinearInterpolationTest, it_linearly_interpolates) {
  NonUniformLinearInterpolation f;

  f.add_point(0, 0);
  f.add_point(2, 1);
  f.add_point(3, 0);
  f.add_point(4,-2);
  f.add_point(8,-1);

  ASSERT_NEAR(f(-1),  0,    0.00001);
  ASSERT_NEAR(f(0),   0,    0.00001);
  ASSERT_NEAR(f(0.5), 0.25, 0.00001);
  ASSERT_NEAR(f(1),   0.5,  0.00001);
  ASSERT_NEAR(f(1.5), 0.75, 0.00001);
  ASSERT_NEAR(f(2),   1,    0.00001);
  ASSERT_NEAR(f(2.5), 0.5,  0.00001);
  ASSERT_NEAR(f(3),   0,    0.00001);
  ASSERT_NEAR(f(3.5),-1,    0.00001);
  ASSERT_NEAR(f(4),  -2,    0.00001);
  ASSERT_NEAR(f(5),  -1.75, 0.00001);
  ASSERT_NEAR(f(8),  -1,    0.00001);
  ASSERT_NEAR(f(9),  -1,    0.00001);
}
