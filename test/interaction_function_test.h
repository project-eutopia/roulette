#pragma once

#include "test_helper.h"

#include "roulette/interaction_function.h"

using namespace roulette;

TEST(InteractionFunctionTest, is_one_minus_exp_of_minus_x) {
  InteractionFunction f{};

  EXPECT_EQ(f(0), 0);

  double x, yreal, yinterp;

  for (x = 0.025; x <= 3; x += 0.025) {
    yreal = 1 - std::exp(-x);
    yinterp = f(x);

    // Expect within 1%
    EXPECT_NEAR(yreal, yinterp, yreal * 0.05);
  }
}
