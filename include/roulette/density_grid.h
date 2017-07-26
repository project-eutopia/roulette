#pragma once

#include <functional>
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
      // VoxelRayTraceIterator is a function that is used to iterate over voxels
      // along a given ray.
      //
      // The return value is the distance processed within this voxel.
      // If the distance is less than the voxel distance, then it terminates.
      //
      // const DensityGrid& is this grid
      // distance is the distance traversed in this voxel
      // zi, yi, xi are the indexes of this voxel
      typedef std::function<double(const DensityGrid& density_grid, double distance, int xi, int yi, int zi)> voxel_iterator;

      DensityGrid(const VoxelGrid& voxel_grid, const ThreeTensor& densities, const Material& material);

      int nz() const;
      int ny() const;
      int nx() const;

      double delta_x() const;
      double delta_y() const;
      double delta_z() const;

      const VoxelGrid& voxel_grid() const;
      double operator()(int xi, int yi, int zi) const;
      const Material& material() const;

      // Photon must be "inside" the grid.  Returns false if transported all the way out.
      bool transport_photon_unitless_depth(Photon* photon, double depth) const;

      // Returns final position
      ThreeVector ray_trace_voxels(const ThreeVector& initial_position, const ThreeVector& direction, voxel_iterator it) const;
  };
};
