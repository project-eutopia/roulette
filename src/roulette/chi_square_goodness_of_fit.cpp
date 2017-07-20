#include "roulette/chi_square_goodness_of_fit.h"

#include <cassert>

namespace roulette {
  ChiSquareGoodnessOfFit::ChiSquareGoodnessOfFit(const Histogram& observed, const Histogram& expected) :
    m_total(observed.total()),
    m_dof(expected.number_of_nonzero_bins() - 1),
    m_chi_square_distribution(m_dof)
  {
    assert(observed.total() == expected.total());
    assert(observed.compatible_with(expected));

    double x;
    m_chi2 = 0;

    for (int i = -1; i <= observed.nbins(); ++i) {
      // Skip bins that should have nothing
      if (expected.count_at(i) == 0) {
        assert(observed.count_at(i) == 0);
        continue;
      }
      x = (observed.count_at(i) - expected.count_at(i));
      m_chi2 += x*x / expected.count_at(i);
    }

    m_probability_at_worse = boost::math::cdf(m_chi_square_distribution, m_chi2);
  }

  double ChiSquareGoodnessOfFit::chi2_value() const { return m_chi2; }
  double ChiSquareGoodnessOfFit::degrees_of_freedom() const { return m_dof; }
  double ChiSquareGoodnessOfFit::probability_of_bad_fit() const { return m_probability_at_worse; }
};
