#pragma once

#include "../test_helper.h"

#include "roulette/random_generator.h"
#include "roulette/histogram.h"
#include "roulette/chi_square_goodness_of_fit.h"
#include "roulette/distributions/exponential.h"

using namespace roulette;

TEST(ExponentialTest, gives_numbers_in_range) {
  RandomGenerator generator;
  distributions::Exponential exponential(1);

  double x;
  for (int i = 0; i < 5; ++i) {
    x = exponential(generator);
    ASSERT_GE(x, 0);
  }
}

TEST(ExponentialTest, has_exponential_distribution) {
  int num_samples = 10000;

  RandomGenerator generator;
  distributions::Exponential exponential(2);
  Histogram expected(0, 10, 25);
  exponential.fill_in_expected_histogram(expected, num_samples);

  std::function<bool(void)> trial([&]() {
    Histogram observed(0, 10, 25);

    for (int i = 0; i < num_samples; ++i) {
      observed.add_point(exponential(generator));
    }

    ChiSquareGoodnessOfFit chi(observed, expected);

    return chi.probability_of_bad_fit() < 0.95;
  });

  // 0.00003% chance of spurious failure
  test_multiple_times(trial, 5);
}

TEST(ExponentialTest, chi_square_test_fails_on_different_exponential_distribution) {
  int num_samples = 10000;

  RandomGenerator generator;

  // Expectation
  distributions::Exponential exponential(2);
  Histogram expected(0, 10, 25);
  exponential.fill_in_expected_histogram(expected, num_samples);

  // Observation
  distributions::Exponential exponential2(2.25);
  Histogram observed(0, 10, 25);
  for (int i = 0; i < num_samples; ++i) {
    observed.add_point(exponential2(generator));
  }

  ChiSquareGoodnessOfFit chi(observed, expected);

  ASSERT_GE(chi.probability_of_bad_fit(), 0.95);
}
