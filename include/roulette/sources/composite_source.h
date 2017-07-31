#pragma once

#include <memory>
#include <vector>

#include "roulette/sources/source.h"
#include "roulette/non_uniform_linear_interpolation.h"

#include "rapidjson/document.h"

namespace roulette {
  class Particle;

  namespace sources {
    class CompositeSource : public Source {
      private:
        std::vector<double> m_fractions;
        std::vector<std::shared_ptr<Source>> m_sub_sources;
        NonUniformLinearInterpolation m_inverse_cdf_to_sub_source_index;

      public:
        CompositeSource(const rapidjson::Value& data);

        std::shared_ptr<Particle> particle(RandomGenerator& generator);
    };
  };
};
