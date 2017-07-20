#pragma once

#include "roulette/real_function.h"
#include "roulette/random_generator.h"
#include "roulette/distribution.h"
#include "roulette/distributions/inverse_cdf_sampling.h"

// Uses the built in distributions, for example normal_distribution, uniform_real_distribution
namespace roulette {
  namespace distributions {
    class ParametricUniform : public Distribution {
      private:
        double m_a;
        double m_b;

        RealFunction m_inverse_cdf;
        InverseCdfSampling m_sampling;

      public:
        ParametricUniform(double initial_a = 0, double initial_b = 1);
        double operator()(RandomGenerator& generator);

        double a() const;
        double b() const;

        void set_a(double a);
        void set_b(double b);
    };
  };
};
