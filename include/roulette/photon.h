#pragma once

#include "roulette/particle.h"

namespace roulette {
  class Photon : public Particle {
    public:
      double mass() const;
      int charge() const;
  };
};
