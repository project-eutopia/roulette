#pragma once

#include <memory>

#include "roulette/sources/source.h"
#include "roulette/distributions/fluence_distribution.h"
#include "roulette/distributions/spectrum.h"
#include "roulette/beam.h"

#include "rapidjson/document.h"

namespace roulette {
  class Particle;

  namespace sources {
    class Source;

    class BeamSource : public Source {
      private:
        Beam m_beam;
        distributions::FluenceDistribution m_fluence;
        distributions::Spectrum m_energy_spectrum;

      public:
        BeamSource(const rapidjson::Value& data);
        // p0, p1, p2 are cyclic corners of square
        BeamSource(const Beam& beam, const distributions::FluenceDistribution& fluence, const distributions::Spectrum& energy_spectrum);

        std::shared_ptr<Particle> particle(RandomGenerator& generator);
    };
  };
};
