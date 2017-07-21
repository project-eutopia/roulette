#pragma once

#include "../test_helper.h"

#include "roulette/random_generator.h"
#include "roulette/histogram.h"
#include "roulette/chi_square_goodness_of_fit.h"
#include "roulette/distributions/compton_scattering.h"

using namespace roulette;

TEST(ComptonScatteringTest, has_compton_scattering_distribution) {
  int num_samples = 1000;

  RandomGenerator generator;
  distributions::ComptonScattering compton;
  compton.set_initial_photon_energy(100);

  Histogram expected(0, 100, 4);
  compton.fill_in_expected_histogram(expected, num_samples);

  std::function<bool(void)> trial([&]() {
    Histogram observed(0, 100, 4);

    for (int i = 0; i < num_samples; ++i) {
      observed.add_point(compton(generator));
    }

    ChiSquareGoodnessOfFit chi(observed, expected);

    return chi.probability_of_bad_fit() < 0.95;
  });

  // 0.00003% chance of spurious failure
  test_multiple_times(trial, 5);
}

TEST(ComptonScatteringTest, chi_square_test_fails_on_different_compton_distribution) {
  int num_samples = 1000;

  RandomGenerator generator;
  distributions::ComptonScattering compton;
  compton.set_initial_photon_energy(100);

  Histogram expected(0, 100, 4);
  compton.fill_in_expected_histogram(expected, num_samples);

  compton.set_initial_photon_energy(90);
  Histogram observed(0, 100, 4);

  for (int i = 0; i < num_samples; ++i) {
    observed.add_point(compton(generator));
  }

  ChiSquareGoodnessOfFit chi(observed, expected);

  ASSERT_GE(chi.probability_of_bad_fit(), 0.95);
}
