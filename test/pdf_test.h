#pragma once

#include "test_helper.h"

#include "roulette/pdf.h"
#include "roulette/chi_square_goodness_of_fit.h"

using namespace roulette;

TEST(PdfTest, pdf_test) {
  std::vector<double> x = {0, 0.1, 0.1, 1.0};
  std::vector<double> y = {1, 1, 0, 0};

  RandomGenerator generator;
  Pdf pdf(x, y);

  for (int i = 0; i < 10; ++i) {
    double res = pdf(generator);
    EXPECT_GE(res, 0);
    EXPECT_LT(res, 0.1);
  }
}

TEST(PdfTest, pdf_back_loaded_test) {
  std::vector<double> x = {0, 0.9, 0.9, 1.0};
  std::vector<double> y = {0, 0, 1, 1};

  RandomGenerator generator;
  Pdf pdf(x, y);

  for (int i = 0; i < 10; ++i) {
    double res = pdf(generator);
    EXPECT_GE(res, 0.9);
    EXPECT_LT(res, 1);
  }
}

TEST(PdfTest, has_expected_distribution_test) {
  int num_samples = 10000;

  RandomGenerator generator;
  std::vector<double> x = {0, 1, 1, 2, 2, 3, 3, 4};
  std::vector<double> y = {0, 0, 1, 1, 3, 3, 0, 0};

  Pdf pdf(x, y);

  Histogram expected(0, 4, 4);

  // Fill in expected values
  for (int i = 0; i < num_samples/4; ++i) {
    expected.add_point(1.5);
  }
  for (int i = 0; i < 3*num_samples/4; ++i) {
    expected.add_point(2.5);
  }

  std::function<bool(void)> trial([&]() {
    Histogram observed(0, 4, 4);

    for (int i = 0; i < num_samples; ++i) {
      observed.add_point(pdf(generator));
    }

    ChiSquareGoodnessOfFit chi(observed, expected);

    return chi.probability_of_bad_fit() < 0.95;
  });

  // 0.00003% chance of spurious failure
  test_multiple_times(trial, 5);
}
