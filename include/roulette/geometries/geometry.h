#pragma once

#include "roulette/random_generator.h"
#include "roulette/three_vector.h"

namespace roulette {
  namespace geometries {
    class InvalidGeometry : public std::runtime_error {
      public:
        InvalidGeometry(const std::string& s) : std::runtime_error(s) {}
    };

    class Geometry {
      public:
        virtual ThreeVector sample(RandomGenerator& generator) const = 0;
    };
  };
};
