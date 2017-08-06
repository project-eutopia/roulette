#pragma once

#include "roulette/beamlet.h"
#include "roulette/sources/source.h"
#include "roulette/distributions/spectrum.h"

#include "rapidjson/document.h"

namespace roulette {
  class Particle;

  namespace sources {
    class DiracDeltaSource : public Source {
      private:
        ThreeVector m_source;
        ThreeVector m_direction;
        distributions::Spectrum m_energy_spectrum;

      public:
        DiracDeltaSource(const rapidjson::Value& data);
        // p0, p1, p2 are cyclic corners of square
        DiracDeltaSource(const ThreeVector& source, const ThreeVector& direction, const distributions::Spectrum& energy_spectrum);

        std::shared_ptr<Particle> particle(RandomGenerator& generator);
    };
  };
};
