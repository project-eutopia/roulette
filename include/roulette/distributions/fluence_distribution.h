#pragma once

#include <vector>
#include "roulette/distributions/probability_sampling.h"
#include "roulette/random_generator.h"
#include "roulette/two_tensor.h"

namespace roulette {
  namespace distributions {
    class FluenceDistribution {
      private:
        int m_nx;
        int m_ny;

        ProbabilitySampling<std::pair<int,int>> m_sampling;

      public:
        FluenceDistribution();
        FluenceDistribution(const TwoTensor& fluence);

        int nx() const;
        int ny() const;

        // (xi, yi) pair of indexes to sample
        const std::pair<int,int>& index(RandomGenerator& generator) const;
    };
  };
};
