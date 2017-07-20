#pragma once
#include <gtest/gtest.h>

#include "roulette/random_generator.h"
#include "roulette/real_function.h"
#include "roulette/distributions/uniform.h"
#include "roulette/distributions/rejection_sampling.h"

using namespace roulette;

TEST(RejectionSamplingTest, generates_variables_according_to_pdf) {
  RandomGenerator generator;

  distributions::RejectionSampling<distributions::Uniform> rejection_sampling(
    std::function<double(double)>([](double x) { return (x < 0.5) ? 0 : 1; }),
    distributions::Uniform(0, 1),
    RealFunction::constant_function(1)
  );

  for (int i = 0; i < 5; ++i) {
    double x = rejection_sampling(generator);
    ASSERT_TRUE(x >= 0.5 && x <= 1.0);
  }
}

TEST(RejectionSamplingTest, it_uses_envelope_when_sampling) {
  RandomGenerator generator;

  distributions::RejectionSampling<distributions::Uniform> same_envelope(
    RealFunction::constant_function(1),
    distributions::Uniform(0, 1),
    RealFunction::constant_function(1)
  );

  for (int i = 0; i < 5; ++i) {
    ASSERT_EQ(same_envelope.sampling_attempts(generator), 1);
  }

  distributions::RejectionSampling<distributions::Uniform> large_envelope(
    RealFunction::constant_function(1),
    distributions::Uniform(0, 1),
    RealFunction::constant_function(10)
  );

  int total = 0;
  for (int i = 0; i < 5; ++i) {
    total += large_envelope.sampling_attempts(generator);
  }
  ASSERT_GT(total, 5);
}
