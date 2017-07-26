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
      double m_weight;

    public:
      Particle(const FourMomentum& momentum, const ThreeVector& position, double weight = 1);
      Particle(double energy, double mass, const ThreeVector& direction, const ThreeVector& position, double weight = 1);

      const FourMomentum& momentum() const;
      FourMomentum& momentum();
      const ThreeVector& position() const;
      ThreeVector& position();

      double energy() const;
      double kinetic_energy() const;

      double weight() const;
      double& weight();

      double virtual mass() const = 0;
      int virtual charge() const = 0;
  };
};

