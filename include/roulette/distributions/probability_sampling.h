
#pragma once

#include <memory>
#include <vector>

#include "roulette/random_generator.h"
#include "roulette/non_uniform_linear_interpolation.h"

#include "rapidjson/document.h"

namespace roulette {
  namespace distributions {
    template <typename T>
    class ProbabilitySampling {
      private:
        NonUniformLinearInterpolation m_inv_cdf_to_indexes;
        std::vector<T> m_values;

      public:
        ProbabilitySampling();
        ProbabilitySampling(const std::vector<double>& probabilities, const std::vector<T>& items);
        const T& operator()(RandomGenerator& generator) const;
    };
  };
};

#include "roulette/distributions/probability_sampling_impl.h"
