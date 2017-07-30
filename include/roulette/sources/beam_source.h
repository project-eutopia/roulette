#pragma once

#include "roulette/sources/source.h"
#include "roulette/beam.h"
#include "roulette/photon.h"
#include "roulette/distributions/fluence_distribution.h"
#include "roulette/distributions/spectrum.h"

#include "rapidjson/document.h"

namespace roulette {
  namespace sources {
    class BeamSource : public Source {
      private:
        Beam m_beam;
        distributions::FluenceDistribution m_fluence;
        distributions::Spectrum m_energy_spectrum;

      public:
        BeamSource(const rapidjson::Value& data);
        // p0, p1, p2 are cyclic corners of square
        BeamSource(const Beam& beam, const distributions::FluenceDistribution& fluence, const distributions::Spectrum& energy_spectrum);

        Photon particle(RandomGenerator& generator);
    };
  };
};
