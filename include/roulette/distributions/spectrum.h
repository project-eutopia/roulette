#pragma once

#include <string>

#include "roulette/random_generator.h"
#include "roulette/non_uniform_linear_interpolation.h"
#include "roulette/distribution.h"

namespace roulette {
  namespace distributions {
    class Spectrum : public Distribution {
      private:
        NonUniformLinearInterpolation m_pdf;
        NonUniformLinearInterpolation m_cdf;
        NonUniformLinearInterpolation m_inv_cdf;

      public:
        Spectrum();
        Spectrum(std::string json_filename);
        double operator()(RandomGenerator& generator);
    };
  };
};
