#pragma once

#include <string>
#include <memory>

#include "roulette/i_voxel_grid.h"
#include "roulette/matrix_three_tensor.h"
#include "roulette/compound.h"
#include "roulette/density_compound_map.h"

#include "rapidjson/document.h"

namespace roulette {
  class ThreeTensor;
  class Compound;
  class Photon;

  class Phantom {
    private:
      std::shared_ptr<const IVoxelGrid> m_voxel_grid;
      std::shared_ptr<const MatrixThreeTensor> m_densities;
      std::vector<std::shared_ptr<const Compound>> m_compounds;

    public:
      Phantom();
      Phantom(const rapidjson::Value& data);
      Phantom(std::shared_ptr<const IVoxelGrid> voxel_grid, std::shared_ptr<const MatrixThreeTensor> densities);

      // Returns a new Phantom object that has the same data as the original, just on
      // a coarser scale (e.g. {2,2,2} means each voxel has 2x2x2 block of voxels within)
      // NOTE:  Only works with regular voxel grids
      Phantom(const Phantom& original_phantom, std::tuple<int,int,int> voxelation_scale);

      void set_compound_map(const DensityCompoundMap& map);

      int nx() const;
      int ny() const;
      int nz() const;

      std::shared_ptr<const MatrixThreeTensor> densities() const;
      std::tuple<int,int,int> index_at(const ThreeVector& position) const;

      const std::shared_ptr<const IVoxelGrid>& voxel_grid() const;
      double operator()(int xi, int yi, int zi) const;
      const Compound& compound(int xi, int yi, int zi) const;
      std::shared_ptr<const Compound> compound_ptr(int xi, int yi, int zi) const;

      // Returns true if still inside, false if transported all the way out.
      bool transport_photon_unitless_depth(Photon& photon, double depth) const;

      // Returns final position
      ThreeVector ray_trace_voxels(const ThreeVector& initial_position, const ThreeVector& direction, IVoxelGrid::voxel_iterator it) const;
  };
};
