#pragma once

#include <boost/multi_array.hpp>

#include "roulette/particle.h"

namespace roulette {
  class VoxelGrid {
    private:
      // Density grid
      /* boost::multi_array<double,3> m_densities; */

      // Lower left point
      ThreeVector m_v0;
      // Upper right point
      ThreeVector m_vn;

      // Number of voxels, lower bound, and voxel width
      unsigned int m_nx;
      unsigned int m_ny;
      unsigned int m_nz;

      double m_delta_x;
      double m_delta_y;
      double m_delta_z;

    public:
      VoxelGrid(/*boost::multi_array<double,3>&& densities,*/ const ThreeVector& v0, const ThreeVector& vn);

      /* unsigned int nz() const; */
      /* unsigned int ny() const; */
      /* unsigned int nx() const; */

      const ThreeVector& v0() const;
      const ThreeVector& vn() const;

      bool inside(const ThreeVector& point) const;
      bool strictly_inside(const ThreeVector& point) const;
      bool outside(const ThreeVector& point) const;

      /* double operator()(unsigned int zi, unsigned int yi, unsigned int xi) const; */

      // Either no intersections (nan, nan)
      // One intersection (t, nan)
      // Two intersections (t1, t2)
      std::pair<double,double> intersection_times(const Particle& particle) const;

      // Returns true if outside particle that intersects surface,
      // and therefore was successfully moved up to surface
      bool transport_particle_to_surface(Particle& particle) const;
  };
};
