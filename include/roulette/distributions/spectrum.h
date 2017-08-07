#pragma once

#include <string>

#include "roulette/random_generator.h"
#include "roulette/non_uniform_linear_interpolation.h"
#include "roulette/distribution.h"

#include "rapidjson/document.h"

namespace roulette {
  namespace distributions {
    class Spectrum : public Distribution {
      private:
        NonUniformLinearInterpolation m_pdf;
        NonUniformLinearInterpolation m_cdf;
        NonUniformLinearInterpolation m_inv_cdf;
        double m_value;

      public:
        Spectrum();
        Spectrum(const rapidjson::Value& data);
        Spectrum(std::string filename_or_json_string);
        double operator()(RandomGenerator& generator);

      private:
        void load_data(const rapidjson::Value& data);
        void load_pdf(const rapidjson::Value& data);
        void load_delta(const rapidjson::Value& data);
    };
  };
};
