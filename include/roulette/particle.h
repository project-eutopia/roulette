#pragma once

#include "roulette/three_vector.h"
#include "roulette/four_momentum.h"
#include "roulette/random_generator.h"
#include "roulette/event.h"
#include "roulette/source_dose.h"
#include "roulette/source_simulation.h"

namespace roulette {
  class Particle {
    protected:
      FourMomentum m_momentum;
      ThreeVector m_position;
      // Stores energy not only implicitly in FourMomentum
      double m_energy;
      double m_weight;

    public:
      enum InteractionType { NONE, PHOTON_SCATTER, PHOTON_PHOTOELECTRIC, ELECTRON_ABSORB };

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

      virtual void deposit_energy(SourceDose& source_dose) = 0;
      virtual std::shared_ptr<Event> simulate(RandomGenerator& generator, const Phantom& phantom) = 0;
  };
};

