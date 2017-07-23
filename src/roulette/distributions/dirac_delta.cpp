#include "roulette/distributions/dirac_delta.h"

#include <algorithm>
#include <limits>
#include <functional>

namespace roulette {
  namespace distributions {
    DiracDelta::DiracDelta(double x) :
      m_x(x)
    {
    };

    double DiracDelta::operator()(RandomGenerator& generator) {
      return m_x;
    };

    double DiracDelta::area_between(double x0, double x1) const {
      if (x0 <= m_x && x1 >= m_x) return 1;
      else return 0;
    }

    double DiracDelta::x() const { return m_x; }
    void DiracDelta::set_x(double x) { m_x = x; }

    void DiracDelta::fill_in_expected_histogram(Histogram& expected, int num_samples) const {
      expected.fill_in_from_cdf(
        std::bind(&DiracDelta::area_between, this, std::placeholders::_1, std::placeholders::_2),
        num_samples
      );
    }
  };
};
