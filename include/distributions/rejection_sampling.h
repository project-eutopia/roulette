#pragma once

#include <memory>

#include "random_generator.h"
#include "real_function.h"
#include "distribution.h"

namespace roulette {
  namespace distributions {
    class RejectionSampling : public Distribution {
      private:
        RealFunction m_pdf;
        std::shared_ptr<Distribution> m_envelope_x_distribution;
        RealFunction m_envelope_y_height;

      public:
        RejectionSampling(RealFunction pdf, std::shared_ptr<Distribution> envelope_x_distribution, RealFunction envelope_y_height);
        double operator()(RandomGenerator& generator);
    };
  };
};
