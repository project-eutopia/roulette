#pragma once

#include "roulette/random_generator.h"
#include "roulette/non_uniform_linear_interpolation.h"
#include "roulette/two_tensor.h"

namespace roulette {
  namespace distributions {
    class FluenceDistribution {
      private:
        int m_nx;
        int m_ny;

        NonUniformLinearInterpolation m_inverse_cdf_to_of_indexes;
        std::vector<std::pair<int,int>> m_indexes;

      public:
        FluenceDistribution(const TwoTensor& fluence);

        int nx() const;
        int ny() const;

        // (xi, yi) pair of indexes to sample
        std::pair<int,int> index(RandomGenerator& generator) const;
    };
  };
};
