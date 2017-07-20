#include "roulette/distributions/sum_of_distributions.h"

namespace roulette {
  namespace distributions {
    template <typename D1, typename D2>
    SumOfDistributions<D1,D2>::SumOfDistributions(D1 distribution1, D2 distribution2, double p1) :
      m_distribution1(distribution1),
      m_distribution2(distribution2),
      m_p1(p1),
      m_p2(1.0 - p1)
    {
    };

    template <typename D1, typename D2>
    double SumOfDistributions<D1,D2>::operator()(RandomGenerator& generator) {
      if (generator.uniform() < m_p1) {
        return m_distribution1(generator);
      } else {
        return m_distribution2(generator);
      }
    };
  };
};
