#pragma once

#include <boost/multi_array.hpp>
#include "roulette/three_vector.h"

#include "rapidjson/document.h"

namespace roulette {
  class Particle;

  class VoxelGrid {
    private:
      int m_nx;
      int m_ny;
      int m_nz;

      // Lower left point
      ThreeVector m_v0;
      // Upper right point
      ThreeVector m_vn;

      double m_delta_x;
      double m_delta_y;
      double m_delta_z;

    public:
      VoxelGrid();
      VoxelGrid(const rapidjson::Value& data);
      VoxelGrid(const ThreeVector& v0, const ThreeVector& vn, int nx, int ny, int nz);

      const ThreeVector& v0() const;
      const ThreeVector& vn() const;

      int nx() const;
      int ny() const;
      int nz() const;

      double delta_x() const;
      double delta_y() const;
      double delta_z() const;

      bool inside(const ThreeVector& point) const;
      bool strictly_inside(const ThreeVector& point) const;
      bool outside(const ThreeVector& point) const;

      // Returns true intersects surface,
      // and therefore was successfully moved up to surface
      bool transport_particle_to_surface(Particle* particle) const;
      bool transport_position_to_surface(ThreeVector& position, const ThreeVector& direction) const;

      // Must be inside and aiming inside to begin with
      double exit_time(const ThreeVector& position, const ThreeVector& velocity) const;

      std::ofstream& write(std::ofstream& os) const;
      std::ifstream& read(std::ifstream& is);
      rapidjson::Value to_json(rapidjson::Document::AllocatorType& allocator) const;
  };
};
