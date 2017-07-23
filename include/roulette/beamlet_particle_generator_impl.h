namespace roulette {
  template <typename PARTICLE, typename ENERGY_DISTRIBUTION>
  BeamletParticleGenerator<PARTICLE,ENERGY_DISTRIBUTION>::BeamletParticleGenerator(const Beamlet& beamlet, const ENERGY_DISTRIBUTION& energy_distribution) :
    m_beamlet(beamlet),
    m_energy_distribution(energy_distribution)
  {
  }

  template <typename PARTICLE, typename ENERGY_DISTRIBUTION>
  PARTICLE BeamletParticleGenerator<PARTICLE,ENERGY_DISTRIBUTION>::generate(RandomGenerator& generator) {
    return PARTICLE(
      FourMomentum(m_energy_distribution(generator), PARTICLE::MASS, m_beamlet.directional_unit_vector(generator)),
      m_beamlet.source()
    );
  }
};
