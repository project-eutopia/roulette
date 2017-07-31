#pragma once

#include "roulette/beamlet.h"
#include "roulette/sources/source.h"
#include "roulette/distributions/spectrum.h"

namespace roulette {
  class Particle;

  namespace sources {
    class BeamletSource : public Source {
      private:
        Beamlet m_beamlet;
        distributions::Spectrum m_energy_spectrum;

      public:
        // p0, p1, p2 are cyclic corners of square
        BeamletSource(const Beamlet& beamlet, const distributions::Spectrum& energy_spectrum);

        std::shared_ptr<Particle> particle(RandomGenerator& generator);
    };
  };
};
