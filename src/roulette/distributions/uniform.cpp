#include "roulette/distributions/uniform.h"

#include <algorithm>
#include <limits>

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
      for (int i = -1; i <= expected.nbins(); ++i) {
        expected.set_bin(i, num_samples * this->area_between(expected.bin_low_x(i), expected.bin_high_x(i)));
      }

      expected.set_total(num_samples);
    }
  };
};
