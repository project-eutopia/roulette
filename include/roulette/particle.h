#pragma once

#include "roulette/three_vector.h"
#include "roulette/four_momentum.h"

namespace roulette {
  class Particle {
    protected:
      FourMomentum m_momentum;
      ThreeVector m_position;
      // Stores energy not only implicitly in FourMomentum
      double m_energy;

    public:
      Particle(const FourMomentum& momentum, const ThreeVector& position);
      Particle(FourMomentum&& momentum, ThreeVector&& position);

      const FourMomentum& momentum() const;
      const ThreeVector& position() const;

      double energy() const;

      double virtual mass() const = 0;
      int virtual charge() const = 0;
  };
};

