#pragma once

#include "roulette/three_vector.h"
#include "roulette/four_momentum.h"

namespace roulette {
  class Particle {
    private:
      // Stores energy not only implicitly in FourMomentum
      double m_energy;
      FourMomentum m_momentum;
      ThreeVector m_position;

    public:
      Particle(const FourMomentum& momentum, const ThreeVector& position);
      Particle(FourMomentum&& momentum, ThreeVector&& position);
      Particle(const Particle& particle);
      Particle(Particle&& particle);

      const FourMomentum& momentum() const;
      const ThreeVector& position() const;

      double energy() const;

      double virtual mass() const = 0;
      int virtual charge() const = 0;
  };
};
