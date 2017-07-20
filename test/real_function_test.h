#pragma once
#include <gtest/gtest.h>

#include "roulette/real_function.h"

using namespace roulette;

TEST(RealFunctionTest, acts_as_given_function) {
  RealFunction f([](double x) { return 2*x + 1; });

  ASSERT_EQ(f(-1), -1);
  ASSERT_EQ(f(0), 1);
  ASSERT_EQ(f(1), 3);
  ASSERT_EQ(f(2), 5);
}

TEST(RealFunctionTest, constant_function_is_constant) {
  RealFunction c = RealFunction::constant_function(99);
  ASSERT_EQ(c(0), 99);
  ASSERT_EQ(c(-3.14), 99);
  ASSERT_EQ(c(1000000), 99);
}
