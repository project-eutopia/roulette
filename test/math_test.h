#pragma once

#include "test_helper.h"

#include "roulette/math.h"
#include <cmath>

using namespace roulette;

TEST(MathTest, test_sin) {
  for (double theta = -0.5; theta < 3*M_PI; theta += 0.15) {
    // Verify within 0.002% of actual value
    EXPECT_LE(std::abs(std::sin(theta) - math::sin(theta)), 0.00002 * std::abs(std::sin(theta)));
  }
}

TEST(MathTest, test_cos) {
  for (double theta = -0.5; theta < 3*M_PI; theta += 0.15) {
    // Verify within 0.002% of actual value
    EXPECT_LE(std::abs(std::cos(theta) - math::cos(theta)), 0.00002 * std::abs(std::cos(theta)));
  }
}
