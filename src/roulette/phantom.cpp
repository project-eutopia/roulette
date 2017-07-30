#include "roulette/phantom.h"

#include <fstream>

namespace roulette {
  Phantom::Phantom(const VoxelGrid& voxel_grid, const ThreeTensor& densities) :
    m_voxel_grid(voxel_grid),
    m_densities(densities)
  {
    m_delta_x = (m_voxel_grid.vn()(0) - m_voxel_grid.v0()(0)) / this->nx();
    m_delta_y = (m_voxel_grid.vn()(1) - m_voxel_grid.v0()(1)) / this->ny();
    m_delta_z = (m_voxel_grid.vn()(2) - m_voxel_grid.v0()(2)) / this->nz();
  }

  int Phantom::nx() const { return m_densities.nx(); }
  int Phantom::ny() const { return m_densities.ny(); }
  int Phantom::nz() const { return m_densities.nz(); }

  double Phantom::delta_x() const { return m_delta_x; }
  double Phantom::delta_y() const { return m_delta_y; }
  double Phantom::delta_z() const { return m_delta_z; }

  const VoxelGrid& Phantom::voxel_grid() const { return m_voxel_grid; }
  double Phantom::operator()(int xi, int yi, int zi) const { return m_densities(xi, yi, zi); }

  std::ofstream& Phantom::write(std::ofstream& os) const {
    m_voxel_grid.write(os);
    m_densities.write(os);
    return os;
  }

  std::ifstream& Phantom::read(std::ifstream& is) {
    m_voxel_grid.read(is);
    m_densities.read(is);
    return is;
  }
};
