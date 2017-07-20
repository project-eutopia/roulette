#pragma once

#include <boost/math/distributions/chi_squared.hpp>

#include "roulette/histogram.h"

namespace roulette {
  class ChiSquareGoodnessOfFit {
    private:
      double m_chi2;
      double m_total;
      double m_dof;

      boost::math::chi_squared m_chi_square_distribution;
      // Smaller probablity means better fit, approaches 1 for absolutely bad
      double m_probability_at_worse;

    public:
      ChiSquareGoodnessOfFit(const Histogram& observed, const Histogram& expected);

      double chi2_value() const;
      double degrees_of_freedom() const;
      double probability_of_bad_fit() const;
  };
};
