#pragma once

#include "roulette/particle.h"
#include "roulette/electron.h"
#include "roulette/source_simulation.h"
#include "roulette/distributions/exponential.h"

namespace roulette {
  class Photon : public Particle {
    // Inherit constructors
    using Particle::Particle;

    private:
      const static distributions::Exponential exponential;

    public:
      constexpr static double MASS = 0;
      constexpr static int CHARGE = 0;

      double mass() const;
      int charge() const;

      // Updates self with new values, and returns the compton scattered electron
      Electron compton_scatter(double photon_energy, double electron_energy, double photon_theta, double electron_theta, double phi);

      void deposit_energy(SourceSimulation& source_simulation);
  };
};

std::ostream &operator<<(std::ostream &os, const roulette::Photon& p);
