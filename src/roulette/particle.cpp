#include "roulette/particle.h"

namespace roulette {
  Particle::Particle(const FourMomentum& momentum, const ThreeVector& position) :
    m_momentum(momentum),
    m_position(position),
    m_energy(m_momentum.energy())
  {
  }

  Particle::Particle(FourMomentum&& momentum, ThreeVector&& position) :
    m_momentum(std::move(momentum)),
    m_position(std::move(position)),
    m_energy(m_momentum.energy())
  {
  }

  Particle::Particle(const Particle& particle) :
    m_momentum(particle.m_momentum),
    m_position(particle.m_position),
    m_energy(m_momentum.energy())
  {
  }

  Particle::Particle(Particle&& particle) :
    m_momentum(std::move(particle.m_momentum)),
    m_position(std::move(particle.m_position)),
    m_energy(m_momentum.energy())
  {
  }

  const FourMomentum& Particle::momentum() const { return m_momentum; }
  const ThreeVector& Particle::position() const { return m_position; }

  double Particle::energy() const { return m_energy; }
};
