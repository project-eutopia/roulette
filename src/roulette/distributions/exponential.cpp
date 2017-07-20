#include "roulette/distributions/exponential.h"

#include <cassert>
#include <cmath>

namespace roulette {
  namespace distributions {
    Exponential::Exponential(double mu) :
      m_mu(mu)
    {
      assert(mu > 0);
    };

    // Inverse CDF is -log(1-x)/mu
    double Exponential::operator()(RandomGenerator& generator) {
      return -std::log(1 - generator.uniform()) / m_mu;
    };
  };
};
