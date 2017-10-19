#pragma once

#include "roulette/random_generator.h"
#include "roulette/three_vector.h"

namespace roulette {
  namespace geometries {
    class Geometry {
      public:
        virtual ThreeVector sample(RandomGenerator& generator) const = 0;
    };
  };
};
