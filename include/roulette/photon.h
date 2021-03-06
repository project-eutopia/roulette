#pragma once

#include "roulette/event.h"
#include "roulette/particle.h"
#include "roulette/electron.h"
#include "roulette/source_dose.h"

#include "roulette/distributions/exponential.h"
#include "roulette/distributions/spherical.h"

namespace roulette {
  class Photon : public Particle {
    // Inherit constructors
    using Particle::Particle;

    private:
      const static distributions::Exponential exponential;
      const static distributions::Spherical spherical;

    public:
      constexpr static double MASS = 0;
      constexpr static int CHARGE = 0;

      double mass() const;
      int charge() const;

      // Updates self with new values, and returns the compton scattered electron
      Electron compton_scatter(double photon_energy, double electron_energy, double photon_theta, double electron_theta, double phi);

      void deposit_energy(SourceDose& source_dose);
      std::shared_ptr<Event> simulate(RandomGenerator& generator, const Phantom& phantom);

    private:
      std::shared_ptr<Event> get_event(RandomGenerator& generator, const Phantom& phantom);
      void event_process(const Event& event, RandomGenerator& generator, std::function<void(Photon&, Electron&)> compton_handler, std::function<void(Electron&)> photoelectric_handler);
  };
};

std::ostream &operator<<(std::ostream &os, const roulette::Photon& p);
