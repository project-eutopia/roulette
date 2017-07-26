#include "roulette/particle.h"

namespace roulette {
  Particle::Particle(const FourMomentum& momentum, const ThreeVector& position, double weight) :
    m_momentum(momentum),
    m_position(position),
    m_energy(m_momentum.energy()),
    m_weight(weight)
  {
  }

  Particle::Particle(double energy, double mass, const ThreeVector& direction, const ThreeVector& position, double weight) :
    m_momentum(energy, mass, direction),
    m_position(position),
    m_energy(energy),
    m_weight(weight)
  {
  }

  const FourMomentum& Particle::momentum() const { return m_momentum; }
  FourMomentum& Particle::momentum() { return m_momentum; }
  const ThreeVector& Particle::position() const { return m_position; }
  ThreeVector& Particle::position() { return m_position; }

  double Particle::energy() const { return m_energy; }

  double Particle::weight() const { return m_weight; }
  double& Particle::weight() { return m_weight; }
};
