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

TEST(UniformTest, chi_square_test_fails_on_different_uniform_distribution) {
  int num_samples = 1000;

  RandomGenerator generator;

  // Expectation
  distributions::Uniform uniform(0, 10);
  Histogram expected(0, 10, 100);
  uniform.fill_in_expected_histogram(expected, num_samples);

  // Observation
  distributions::Uniform uniform1(1, 10);
  Histogram observed1(0, 10, 100);
  for (int i = 0; i < num_samples; ++i) {
    observed1.add_point(uniform1(generator));
  }

  ChiSquareGoodnessOfFit chi_1(observed1, expected);

  ASSERT_GE(chi_1.probability_of_bad_fit(), 0.95);

  // Observation
  distributions::Uniform uniform2(1, 10);
  Histogram observed2(0, 10, 100);
  for (int i = 0; i < num_samples; ++i) {
    observed2.add_point(uniform2(generator));
  }

  ChiSquareGoodnessOfFit chi_2(observed2, expected);

  ASSERT_GE(chi_2.probability_of_bad_fit(), 0.95);
}

TEST(UniformTest, can_change_range_dynamically) {
  RandomGenerator generator;
  distributions::Uniform uniform(0, 1);

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
