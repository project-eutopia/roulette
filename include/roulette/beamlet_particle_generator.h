#pragma once

#include "roulette/beamlet.h"

namespace roulette {
  template <typename PARTICLE, typename ENERGY_DISTRIBUTION>
  class BeamletParticleGenerator {
    private:
      Beamlet m_beamlet;
      ENERGY_DISTRIBUTION m_energy_distribution;

    public:
      BeamletParticleGenerator(const Beamlet& beamlet, const ENERGY_DISTRIBUTION& energy_distribution);
      PARTICLE generate(RandomGenerator& generator);
  };
};

#include "roulette/beamlet_particle_generator_impl.h"
