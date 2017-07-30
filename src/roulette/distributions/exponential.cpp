#include "roulette/distributions/exponential.h"

#include <cassert>
#include <cmath>
#include <algorithm>
#include <functional>

namespace roulette {
  namespace distributions {
    Exponential::Exponential(double mu) :
      m_mu(mu)
    {
      assert(mu > 0);
    };

    // Inverse CDF is -log(1-x)/mu
    double Exponential::operator()(RandomGenerator& generator) const {
      return -std::log(1 - generator.uniform()) / m_mu;
    };

    // CDF is 1 - exp(-mu*x)
    double Exponential::area_between(double x0, double x1) const {
      // Only valid for non-negative numbers
      x0 = std::max(x0, 0.0);
      x1 = std::max(x1, 0.0);
      return std::exp(-m_mu*x0) - std::exp(-m_mu*x1);
    }

    void Exponential::fill_in_expected_histogram(Histogram& expected, int num_samples) const {
      expected.fill_in_from_cdf(
        std::bind(&Exponential::area_between, this, std::placeholders::_1, std::placeholders::_2),
        num_samples
      );
    }

    double Exponential::mu() const { return m_mu; }
    void Exponential::set_mu(double mu) { m_mu = mu; }
  };
};
