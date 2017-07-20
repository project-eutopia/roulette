#pragma once

#include <memory>

#include "roulette/random_generator.h"
#include "roulette/real_function.h"
#include "roulette/distribution.h"

namespace roulette {
  namespace distributions {
    template <typename D>
    class RejectionSampling : public Distribution {
      private:
        RealFunction m_pdf;
        D m_envelope_x_distribution;
        RealFunction m_envelope_y_height;

      public:
        RejectionSampling(RealFunction pdf, D envelope_x_distribution, RealFunction envelope_y_height);
        double operator()(RandomGenerator& generator);
        // Useful for performance testing -- returns 1 + number of rejected samples
        int sampling_attempts(RandomGenerator& generator);
    };
  };
};

#include "roulette/distributions/rejection_sampling_impl.h"
