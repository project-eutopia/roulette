#pragma once

#include <string>

#include "roulette/voxel_grid.h"
#include "roulette/three_tensor.h"

namespace roulette {
  class Phantom {
    private:
      VoxelGrid m_voxel_grid;
      ThreeTensor m_densities;

      double m_delta_x;
      double m_delta_y;
      double m_delta_z;

    public:
      Phantom();
      Phantom(const VoxelGrid& voxel_grid, const ThreeTensor& densities);

      int nx() const;
      int ny() const;
      int nz() const;

      double delta_x() const;
      double delta_y() const;
      double delta_z() const;

      const VoxelGrid& voxel_grid() const;
      double operator()(int xi, int yi, int zi) const;

      std::ofstream& write(std::ofstream& os) const;
      std::ifstream& read(std::ifstream& is);
  };
};
