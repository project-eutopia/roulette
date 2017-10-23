#pragma once

#include "roulette/i_voxel_grid.h"

#include <vector>

#include "rapidjson/document.h"

namespace roulette {
  class Particle;
  class Phantom;

  class IrregularVoxelGrid : public IVoxelGrid {
    private:
      std::vector<double> m_xplanes;
      std::vector<double> m_yplanes;
      std::vector<double> m_zplanes;

      size_t m_nx;
      size_t m_ny;
      size_t m_nz;

      // Lower left point
      ThreeVector m_v0;
      // Upper right point
      ThreeVector m_vn;

    public:
      IrregularVoxelGrid();
      IrregularVoxelGrid(const rapidjson::Value& data);
      IrregularVoxelGrid(const std::vector<double>& xplanes, const std::vector<double>& yplanes, const std::vector<double>& zplanes);

      const ThreeVector& v0() const;
      const ThreeVector& vn() const;

      size_t nx() const;
      size_t ny() const;
      size_t nz() const;

      std::tuple<size_t,size_t,size_t> index_at(const ThreeVector& position) const;

      rapidjson::Value to_json(rapidjson::Document::AllocatorType& allocator) const;

      // For processing voxels in a ray, where the processing is done by the voxel_iterator.
      // Returns final position
      ThreeVector ray_trace_voxels(const ThreeVector& initial_position, const ThreeVector& direction, IVoxelGrid::voxel_iterator it) const;

    private:
      void validate_planes() const;
  };
};
