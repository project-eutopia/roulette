#pragma once

#include "roulette/random_generator.h"
#include "roulette/three_vector.h"

namespace roulette {
  namespace distributions {
    class Spherical {
      public:
        Spherical();
        ThreeVector operator()(RandomGenerator& generator);
    };
  };
};
