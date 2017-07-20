#pragma once
#include <gtest/gtest.h>

#include "roulette/random_generator.h"
#include "roulette/distributions/parametric_uniform.h"

using namespace roulette;

TEST(UniformTest, can_change_range_dynamically) {
  RandomGenerator generator;
  distributions::ParametricUniform uniform(0, 1);

  double x;
  for (int i = 0; i < 5; ++i) {
    x = uniform(generator);
    ASSERT_GE(x, 0);
    ASSERT_LT(x, 1);
  }

  uniform.set_a(100);
  uniform.set_b(200);

  for (int i = 0; i < 5; ++i) {
    x = uniform(generator);
    ASSERT_GE(x, 100);
    ASSERT_LT(x, 200);
  }
}
