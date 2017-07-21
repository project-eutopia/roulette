#include "roulette/distributions/uniform.h"

#include <algorithm>
#include <limits>
#include <functional>

namespace roulette {
  namespace distributions {
    Uniform::Uniform(double a, double b) :
      m_a(a),
      m_b(b)
    {
    };

    double Uniform::operator()(RandomGenerator& generator) {
      return m_a + (m_b - m_a) * generator.uniform();
    };

    double Uniform::area_between(double x0, double x1) const {
      x0 = std::max(x0, m_a);
      x1 = std::min(x1, m_b);

      return (x1 > x0) ? (x1 - x0) / (m_b - m_a) : 0;
    }

    void Uniform::fill_in_expected_histogram(Histogram& expected, int num_samples) const {
      expected.fill_in_from_cdf(
        std::bind(&Uniform::area_between, this, std::placeholders::_1, std::placeholders::_2),
        num_samples
      );
    }
  };
};
