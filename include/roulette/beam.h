#pragma once

#include "roulette/random_generator.h"
#include "roulette/three_vector.h"

namespace roulette {
  class Beam {
    private:
      ThreeVector m_source;

      ThreeVector m_p0;
      ThreeVector m_u1;
      ThreeVector m_u2;

      int m_nx;
      int m_ny;

    public:
      // p0, p1, p2 are cyclic corners of square
      Beam(const ThreeVector& source, const ThreeVector& p0, const ThreeVector& p1, const ThreeVector& p2, int nx, int ny);

      // Generates a random unit vector that points from the source, to a uniformly selected random
      // point within the beamlet square specified by xi and yi
      ThreeVector directional_unit_vector(RandomGenerator& generator, int xi, int yi) const;

      int nx() const;
      int ny() const;

      const ThreeVector& source() const;
  };
};
