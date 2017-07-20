#pragma once

#include "roulette/random_generator.h"
#include "roulette/distribution.h"

namespace roulette {
  namespace distributions {
    // PDF:  mu exp(-mu * x) for x in [0,inf)
    // mu must be positive
    class Exponential : public Distribution {
      private:
        double m_mu;

      public:
        Exponential(double mu);
        double operator()(RandomGenerator& generator);
    };
  };
};
