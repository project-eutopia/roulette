#pragma once
#include <gtest/gtest.h>

#include "roulette/random_generator.h"
#include "roulette/distributions/uniform.h"

using namespace roulette;

TEST(UniformTest, gives_numbers_in_range) {
  RandomGenerator generator;
  distributions::Uniform uniform(99, 100);

  double x;
  for (int i = 0; i < 5; ++i) {
    x = uniform(generator);
    ASSERT_GE(x, 99);
    ASSERT_LT(x, 100);
  }
}
