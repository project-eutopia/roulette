#pragma once

#include "distribution.h"

// Uses the built in distributions, for example normal_distribution, uniform_real_distribution
namespace roulette {
  namespace distributions {
    template <class D>
    class BuiltIn : public Distribution {
      private:
        D m_distribution;

      public:
        BuiltIn(D distribution) : m_distribution(distribution) {};
        double operator()(RandomGenerator& generator) {
          return m_distribution(generator.engine());
        }
    };
  };
};
