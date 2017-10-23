#pragma once

#include "roulette/sources/source.h"
#include "roulette/geometries/geometry.h"
#include "roulette/distributions/spectrum.h"

#include "rapidjson/document.h"

namespace roulette {
  class Particle;

  namespace sources {
    class BifocalSource : public Source {
      private:
        std::shared_ptr<const geometries::Geometry> m_origin;
        std::shared_ptr<const geometries::Geometry> m_secondary;
        bool m_secondary_is_aperture_generator;
        distributions::Spectrum m_energy_spectrum;

      public:
        BifocalSource(const rapidjson::Value& data);

        std::shared_ptr<Particle> particle(RandomGenerator& generator);
    };
  };
};
