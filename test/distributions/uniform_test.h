#pragma once

#include "../test_helper.h"

#include "roulette/random_generator.h"
#include "roulette/histogram.h"
#include "roulette/chi_square_goodness_of_fit.h"
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

TEST(UniformTest, has_uniform_distribution) {
  int num_samples = 1000;

  RandomGenerator generator;
  distributions::Uniform uniform(0, 10);
  Histogram expected(0, 10, 100);
  uniform.fill_in_expected_histogram(expected, num_samples);

  std::function<bool(void)> trial([&]() {
    Histogram observed(0, 10, 100);

    for (int i = 0; i < num_samples; ++i) {
      observed.add_point(uniform(generator));
    }

    ChiSquareGoodnessOfFit chi(observed, expected);

    return chi.probability_of_bad_fit() < 0.95;
  });

  // 0.00003% chance of spurious failure
  test_multiple_times(trial, 5);
}
