#pragma once

#include "roulette/random_generator.h"
#include "roulette/distribution.h"

namespace roulette {
  namespace distributions {
    class Uniform : public Distribution {
      private:
        double m_a;
        double m_b;

      public:
        Uniform(double a = 0, double b = 1);
        double operator()(RandomGenerator& generator);
    };
  };
};
