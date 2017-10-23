#pragma once

#include "roulette/i_voxel_grid.h"

#include "rapidjson/document.h"

namespace roulette {
  class Particle;
  class Phantom;

  class RegularVoxelGrid : public IVoxelGrid {
    private:
      size_t m_nx;
      size_t m_ny;
      size_t m_nz;

      // Lower left point
      ThreeVector m_v0;
      // Upper right point
      ThreeVector m_vn;

      double m_delta_x;
      double m_delta_y;
      double m_delta_z;

    public:
      RegularVoxelGrid();
      RegularVoxelGrid(const rapidjson::Value& data);
      RegularVoxelGrid(const ThreeVector& v0, const ThreeVector& vn, int nx, int ny, int nz);

      const ThreeVector& v0() const;
      const ThreeVector& vn() const;

      size_t nx() const;
      size_t ny() const;
      size_t nz() const;

      double delta_x() const;
      double delta_y() const;
      double delta_z() const;

      std::tuple<size_t,size_t,size_t> index_at(const ThreeVector& position) const;
      std::tuple<double,double,double> normal_coordinates(const ThreeVector& position) const;

      rapidjson::Value to_json(rapidjson::Document::AllocatorType& allocator) const;

      // For processing voxels in a ray, where the processing is done by the voxel_iterator.
      // Returns final position
      ThreeVector ray_trace_voxels(const ThreeVector& initial_position, const ThreeVector& direction, IVoxelGrid::voxel_iterator it) const;
  };
};
