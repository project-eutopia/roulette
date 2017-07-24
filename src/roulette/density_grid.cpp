#include "roulette/density_grid.h"

#include <cassert>

namespace roulette {
  DensityGrid::DensityGrid(const VoxelGrid& voxel_grid, const boost::multi_array<double,3>& densities, const Material& material) :
    m_voxel_grid(voxel_grid),
    m_densities(densities),
    m_material(material)
  {
  }

  int DensityGrid::nz() const { return m_densities.shape()[0]; }
  int DensityGrid::ny() const { return m_densities.shape()[1]; }
  int DensityGrid::nx() const { return m_densities.shape()[2]; }

  const VoxelGrid& DensityGrid::voxel_grid() const { return m_voxel_grid; }
  double DensityGrid::operator()(int zi, int yi, int xi) const { return m_densities[zi][yi][xi]; }
  const Material& DensityGrid::material() const { return m_material; }
};
