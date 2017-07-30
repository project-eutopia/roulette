#pragma once

#include "roulette/particle.h"
#include "roulette/source_simulation.h"

#define ELECTRON_MASS_IN_EV 510998.9461
#define ELECTRON_REST_ENERGY_IN_EV ELECTRON_MASS_IN_EV

namespace roulette {
  class Electron : public Particle {
    // Inherit constructors
    using Particle::Particle;

    public:
      constexpr static double MASS = ELECTRON_MASS_IN_EV;
      constexpr static int CHARGE = -1;

      double mass() const;
      int charge() const;

      void deposit_energy(SourceSimulation& source_simulation);
  };
};

std::ostream &operator<<(std::ostream &os, const roulette::Electron& e);
