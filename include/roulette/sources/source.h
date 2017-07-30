#pragma once

#include <memory>
#include "roulette/random_generator.h"

namespace roulette {
  class Particle;

  namespace sources {
    class Source {
      public:
        virtual std::shared_ptr<Particle> particle(RandomGenerator& generator) = 0;
    };
  };
};
