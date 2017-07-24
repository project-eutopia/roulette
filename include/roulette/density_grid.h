#pragma once

#include <boost/multi_array.hpp>

#include "roulette/voxel_grid.h"
#include "roulette/material.h"

namespace roulette {
  class DensityGrid {
    private:
      VoxelGrid m_voxel_grid;
      // Shape (nz, ny, nx)
      boost::multi_array<double,3> m_densities;
      // For now, use single material
      Material m_material;

    public:
      DensityGrid(const VoxelGrid& voxel_grid, const boost::multi_array<double,3>& densities, const Material& material);

      int nz() const;
      int ny() const;
      int nx() const;

      const VoxelGrid& voxel_grid() const;
      double operator()(int zi, int yi, int xi) const;
      const Material& material() const;
  };
};
