#pragma once

#include "roulette/random_generator.h"
#include "roulette/real_function.h"
#include "roulette/distribution.h"

namespace roulette {
  namespace distributions {
    class InverseCdfSampling : public Distribution {
      private:
        RealFunction m_inv_cdf;

      public:
        InverseCdfSampling(RealFunction inv_cdf);
        double operator()(RandomGenerator& generator);
    };
  };
};
