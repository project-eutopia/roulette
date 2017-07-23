#pragma once

#include <boost/multi_array.hpp>

namespace roulette {
  class VoxelGrid {
    private:
      // Number of voxels, lower bound, and voxel width
      unsigned int m_nx;
      double m_x0;
      double m_delta_x;

      unsigned int m_ny;
      double m_y0;
      double m_delta_y;

      unsigned int m_nz;
      double m_z0;
      double m_delta_z;

      boost::multi_array<double,3> m_densities;

      VoxelGrid(unsigned int nx, double x0, double delta_x, unsigned int ny, double y0, double delta_y, unsigned int nz, double z0, double delta_z, const boost::multi_array<double,3>& densities);
      VoxelGrid(unsigned int nx, double x0, double delta_x, unsigned int ny, double y0, double delta_y, unsigned int nz, double z0, double delta_z, boost::multi_array<double,3>&& densities);

      unsigned int nx() const;
      // Lower boundary
      double x0() const;
      // Upper bound
      double xn() const;
      double delta_x() const;

      unsigned int ny() const;
      // Lower boundary
      double y0() const;
      double yn() const;
      double delta_y() const;

      unsigned int nz() const;
      // Lower boundary
      double z0() const;
      // Upper bound
      double zn() const;
      double delta_z() const;

      double operator()(unsigned int zi, unsigned int yi, unsigned int xi) const;

    private:
      bool valid() const;
  };
};
