#pragma once

#include <memory>

#include "roulette/real_function.h"
#include "roulette/random_generator.h"
#include "roulette/distribution.h"

// Uses the built in distributions, for example normal_distribution, uniform_real_distribution
namespace roulette {
  namespace distributions {
    template <typename D1, typename D2>
    class SumOfDistributions : public Distribution {
      private:
        D1 m_distribution1;
        D2 m_distribution2;
        double m_p1;
        double m_p2;

      public:
        SumOfDistributions(D1 distribution1, D2 distribution2, double p1);
        double operator()(RandomGenerator& generator);
    };
  };
};

#include "roulette/distributions/sum_of_distributions_impl.h"
