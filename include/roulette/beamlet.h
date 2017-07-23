#pragma once

#include "roulette/random_generator.h"
#include "roulette/three_vector.h"

namespace roulette {
  class Beamlet {
    private:
      ThreeVector m_source;

      ThreeVector m_p0;
      ThreeVector m_u1;
      ThreeVector m_u2;

    public:
      // p0, p1, p2 are cyclic corners of square
      Beamlet(const ThreeVector& source, const ThreeVector& p0, const ThreeVector& p1, const ThreeVector& p2);

      // Generates a random unit vector that points from the source, to a uniformly selected random
      // point within the beamlet square
      ThreeVector directional_unit_vector(RandomGenerator& generator) const;

      const ThreeVector& source() const;
  };
};
