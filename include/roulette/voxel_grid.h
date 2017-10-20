#pragma once

#include "roulette/i_voxel_grid.h"

#include "rapidjson/document.h"

namespace roulette {
  class Particle;
  class Phantom;

  class VoxelGrid : public IVoxelGrid {
    public:
      // voxel_iterator is a function that is used to iterate over voxels
      // along a given ray.
      //
      // The function argument distance is the maximum distance to be traveled in this voxel.
      // The return value of this functionshould be the distance processed within this voxel,
      // and if this is less than the maximum distance, then voxel iteration terminates.
      //
      // distance is the maximum distance to traverse in this voxel
      // zi, yi, xi are the indexes of this voxel
      typedef std::function<double(double distance, int xi, int yi, int zi)> voxel_iterator;

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

      std::tuple<int,int,int> index_at(const ThreeVector& position) const;
      std::tuple<double,double,double> normal_coordinates(const ThreeVector& position) const;

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

      // For processing voxels in a ray, where the processing is done by the voxel_iterator.
      // Returns final position
      ThreeVector ray_trace_voxels(const ThreeVector& initial_position, const ThreeVector& direction, voxel_iterator it) const;
  };
};
