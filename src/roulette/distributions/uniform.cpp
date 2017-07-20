#include "roulette/distributions/uniform.h"

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
  };
};
