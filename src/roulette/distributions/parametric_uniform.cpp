#include "roulette/distributions/parametric_uniform.h"

namespace roulette {
  namespace distributions {
    ParametricUniform::ParametricUniform(double initial_a, double initial_b) :
      m_a(initial_a),
      m_b(initial_b),
      m_inverse_cdf(
        [this](double u) {
          return this->m_a + (this->m_b - this->m_a) * u;
        }
      ),
      m_sampling(m_inverse_cdf)
    {
    };

    double ParametricUniform::operator()(RandomGenerator& generator) {
      return m_sampling(generator);
    };

    double ParametricUniform::a() const { return m_a; }
    double ParametricUniform::b() const { return m_b; }
    void ParametricUniform::set_a(double a) { m_a = a; }
    void ParametricUniform::set_b(double b) { m_b = b; }
  };
};
