#pragma once

#include <string>

#include "roulette/voxel_grid.h"
#include "roulette/three_tensor.h"
#include "roulette/compound.h"

namespace roulette {
  class VoxelGrid;
  class ThreeTensor;
  class Compound;
  class Photon;

  class Phantom {
    private:
      VoxelGrid m_voxel_grid;
      ThreeTensor m_densities;
      Compound m_compound;

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
      // const Phantom& is this grid
      // distance is the distance traversed in this voxel
      // zi, yi, xi are the indexes of this voxel
      typedef std::function<double(const Phantom& phantom, double distance, int xi, int yi, int zi)> voxel_iterator;

      Phantom();
      Phantom(std::string filename);
      Phantom(const VoxelGrid& voxel_grid, const ThreeTensor& densities);

      void set_compound(const Compound& compound);
      int nx() const;
      int ny() const;
      int nz() const;

      double delta_x() const;
      double delta_y() const;
      double delta_z() const;

      std::tuple<int,int,int> index_at(const ThreeVector& position) const;

      const VoxelGrid& voxel_grid() const;
      double operator()(int xi, int yi, int zi) const;
      const Compound& compound(int xi, int yi, int zi) const;

      // Returns false if transported all the way out.
      bool transport_photon_unitless_depth(Photon& photon, double depth) const;

      // Returns final position
      ThreeVector ray_trace_voxels(const ThreeVector& initial_position, const ThreeVector& direction, voxel_iterator it) const;

      std::ofstream& write(std::ofstream& os) const;
      std::ifstream& read(std::ifstream& is);
  };
};
