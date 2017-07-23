#include "roulette/voxel_grid.h"

#include <cassert>

namespace roulette {
  VoxelGrid::VoxelGrid(unsigned int nx, double x0, double delta_x, unsigned int ny, double y0, double delta_y, unsigned int nz, double z0, double delta_z, const boost::multi_array<double,3>& densities) :
    m_nx(nx),
    m_x0(x0),
    m_delta_x(delta_x),
    m_ny(ny),
    m_y0(y0),
    m_delta_y(delta_y),
    m_nz(nz),
    m_z0(z0),
    m_delta_z(delta_z),
    m_densities(densities)
  {
    assert(valid());
  }

  VoxelGrid::VoxelGrid(unsigned int nx, double x0, double delta_x, unsigned int ny, double y0, double delta_y, unsigned int nz, double z0, double delta_z, boost::multi_array<double,3>&& densities) :
    m_nx(nx),
    m_x0(x0),
    m_delta_x(delta_x),
    m_ny(ny),
    m_y0(y0),
    m_delta_y(delta_y),
    m_nz(nz),
    m_z0(z0),
    m_delta_z(delta_z),
    m_densities(densities)
  {
    assert(valid());
  }

  unsigned int VoxelGrid::nx() const { return m_nx; }
  double VoxelGrid::x0() const { return m_x0; }
  double VoxelGrid::xn() const { return m_x0 + m_nx*m_delta_x; }
  double VoxelGrid::delta_x() const { return m_delta_x; }

  unsigned int VoxelGrid::ny() const { return m_ny; }
  double VoxelGrid::y0() const { return m_y0; }
  double VoxelGrid::yn() const { return m_y0 + m_ny*m_delta_y; }
  double VoxelGrid::delta_y() const { return m_delta_y; }

  unsigned int VoxelGrid::nz() const { return m_nz; }
  double VoxelGrid::z0() const { return m_z0; }
  double VoxelGrid::zn() const { return m_z0 + m_nz*m_delta_z; }
  double VoxelGrid::delta_z() const { return m_delta_z; }

  double VoxelGrid::operator()(unsigned int zi, unsigned int yi, unsigned int xi) const {
    return m_densities[zi][yi][xi];
  }

  bool VoxelGrid::valid() const {
    return (
      (m_nz > 0) &&
      (m_ny > 0) &&
      (m_nx > 0) &&
      (m_nz == m_densities.shape()[0]) &&
      (m_ny == m_densities.shape()[1]) &&
      (m_nx == m_densities.shape()[2])
    );
  }
};
