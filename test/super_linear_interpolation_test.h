#pragma once

#include "test_helper.h"

#include "roulette/super_linear_interpolation.h"

using namespace roulette;

TEST(SuperLinearInterpolationTest, test_super_linear_interpolation) {
  SuperLinearInterpolation<std::function<double(double)>> f(
    [](double x) { return 1 - std::exp(-x); },
    [](double x) { return x + std::exp(-x); },
    [](double x) { return 0.5*x*x + std::exp(-x)*(1+x); },
    0,
    1,
    5
  );

  for (double x = -0.5; x < 1.5; x += 0.025) {
    EXPECT_LE(std::abs(1 - std::exp(-x) - f(x)), 0.005);
  }
}
