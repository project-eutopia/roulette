#pragma once

#include "roulette/beamlet.h"
#include "roulette/photon.h"
#include "roulette/distributions/spectrum.h"

namespace roulette {
  namespace sources {
    class BeamletSource {
      private:
        Beamlet m_beamlet;
        distributions::Spectrum m_energy_spectrum;

      public:
        // p0, p1, p2 are cyclic corners of square
        BeamletSource(const Beamlet& beamlet, const distributions::Spectrum& energy_spectrum);

        Photon particle(RandomGenerator& generator);
    };
  };
};
