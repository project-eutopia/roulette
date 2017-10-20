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
        std::shared_ptr<const geometries::Geometry> m_direction;
        distributions::Spectrum m_energy_spectrum;

      public:
        BifocalSource(const rapidjson::Value& data);
        BifocalSource(const std::shared_ptr<const geometries::Geometry> origin, const std::shared_ptr<const geometries::Geometry> direction, const distributions::Spectrum& energy_spectrum);

        std::shared_ptr<Particle> particle(RandomGenerator& generator);
    };
  };
};
