#pragma once

#include "roulette/particle.h"

namespace roulette {
  class Photon : public Particle {
    // Inherit constructors
    using Particle::Particle;

    public:
      double mass() const;
      int charge() const;
  };
};
