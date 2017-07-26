#pragma once

#include <boost/multi_array.hpp>

#include "roulette/voxel_grid.h"
#include "roulette/three_tensor.h"
#include "roulette/material.h"
#include "roulette/photon.h"

namespace roulette {
  class DensityGrid {
    private:
      VoxelGrid m_voxel_grid;
      ThreeTensor m_densities;
      // For now, use single material
      Material m_material;

      double m_delta_x;
      double m_delta_y;
      double m_delta_z;

    public:
      DensityGrid(const VoxelGrid& voxel_grid, const ThreeTensor& densities, const Material& material);

      int nz() const;
      int ny() const;
      int nx() const;

      double delta_x() const;
      double delta_y() const;
      double delta_z() const;

      const VoxelGrid& voxel_grid() const;
      double operator()(int zi, int yi, int xi) const;
      const Material& material() const;

      // Photon must be "inside" the grid.  Returns false if transported all the way out.
      bool transport_photon_unitless_depth(Photon* photon, double depth) const;
  };
};
