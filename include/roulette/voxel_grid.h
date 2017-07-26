#pragma once

#include <boost/multi_array.hpp>

#include "roulette/particle.h"

namespace roulette {
  class VoxelGrid {
    private:
      // Lower left point
      ThreeVector m_v0;
      // Upper right point
      ThreeVector m_vn;

    public:
      VoxelGrid(const ThreeVector& v0, const ThreeVector& vn);

      const ThreeVector& v0() const;
      const ThreeVector& vn() const;

      bool inside(const ThreeVector& point) const;
      bool strictly_inside(const ThreeVector& point) const;
      bool outside(const ThreeVector& point) const;

      // Returns true intersects surface,
      // and therefore was successfully moved up to surface
      bool transport_particle_to_surface(Particle* particle) const;
      bool transport_position_to_surface(ThreeVector& position, const ThreeVector& direction) const;

      // Must be inside and aiming inside to begin with
      double exit_time(const ThreeVector& position, const ThreeVector& velocity) const;
  };
};
