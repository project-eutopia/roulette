#include "roulette/voxel_grid.h"

#include <cassert>
#include <cmath>

namespace roulette {
  VoxelGrid::VoxelGrid(/*boost::multi_array<double,3>&& densities,*/ const ThreeVector& v0, const ThreeVector& vn) :
    /* m_densities(densities), */
    m_v0(v0),
    m_vn(vn)
  {
  }

  /* unsigned int VoxelGrid::nz() const { return m_densities.shape()[0]; } */
  /* unsigned int VoxelGrid::ny() const { return m_densities.shape()[1]; } */
  /* unsigned int VoxelGrid::nx() const { return m_densities.shape()[2]; } */

  const ThreeVector& VoxelGrid::v0() const { return m_v0; }
  const ThreeVector& VoxelGrid::vn() const { return m_vn; }

  bool VoxelGrid::inside(const ThreeVector& point) const {
    return (
      point(0) >= m_v0(0) && point(0) <= m_vn(0) &&
      point(1) >= m_v0(1) && point(1) <= m_vn(1) &&
      point(2) >= m_v0(2) && point(2) <= m_vn(2)
    );
  }

  bool VoxelGrid::strictly_inside(const ThreeVector& point) const {
    return (
      point(0) > m_v0(0) && point(0) < m_vn(0) &&
      point(1) > m_v0(1) && point(1) < m_vn(1) &&
      point(2) > m_v0(2) && point(2) < m_vn(2)
    );
  }

  bool VoxelGrid::outside(const ThreeVector& point) const {
    return !this->inside(point);
  }

  /* double VoxelGrid::operator()(unsigned int zi, unsigned int yi, unsigned int xi) const { */
  /*   return m_densities[zi][yi][xi]; */
  /* } */

  std::pair<double,double> VoxelGrid::intersection_times(const Particle& particle) const {
    double u_dot_x_normal = particle.momentum()(1);
    double u_dot_y_normal = particle.momentum()(2);
    double u_dot_z_normal = particle.momentum()(3);

    std::vector<double> times;

    double t;
    double x, y, z;

    // Low x plane
    t = (m_v0(0) - particle.position()(0)) / u_dot_x_normal;
    if (std::isfinite(t) && t >= 0) {
      y = particle.position()(1) + t * particle.momentum()(2);
      z = particle.position()(2) + t * particle.momentum()(3);

      if (y >= m_v0(1) && y <= m_vn(1) && z >= m_v0(2) && z <= m_vn(2)) {
        times.push_back(t);
      }
    }

    // High x plane
    t = (m_vn(0) - particle.position()(0)) / u_dot_x_normal;
    if (std::isfinite(t) && t >= 0) {
      y = particle.position()(1) + t * particle.momentum()(2);
      z = particle.position()(2) + t * particle.momentum()(3);

      if (y >= m_v0(1) && y <= m_vn(1) && z >= m_v0(2) && z <= m_vn(2)) {
        times.push_back(t);
      }
    }

    // Low y plane
    t = (m_v0(1) - particle.position()(1)) / u_dot_y_normal;
    if (std::isfinite(t) && t >= 0) {
      x = particle.position()(0) + t * particle.momentum()(1);
      z = particle.position()(2) + t * particle.momentum()(3);

      if (x >= m_v0(0) && x <= m_vn(0) && z >= m_v0(2) && z <= m_vn(2)) {
        times.push_back(t);
      }
    }

    // High y plane
    t = (m_vn(1) - particle.position()(1)) / u_dot_y_normal;
    if (std::isfinite(t) && t >= 0) {
      x = particle.position()(0) + t * particle.momentum()(1);
      z = particle.position()(2) + t * particle.momentum()(3);

      if (x >= m_v0(0) && x <= m_vn(0) && z >= m_v0(2) && z <= m_vn(2)) {
        times.push_back(t);
      }
    }

    // Low z plane
    t = (m_v0(2) - particle.position()(2)) / u_dot_z_normal;
    if (std::isfinite(t) && t >= 0) {
      x = particle.position()(0) + t * particle.momentum()(1);
      y = particle.position()(1) + t * particle.momentum()(2);

      if (x >= m_v0(0) && x <= m_vn(0) && y >= m_v0(1) && y <= m_vn(1)) {
        times.push_back(t);
      }
    }

    // High z plane
    t = (m_vn(2) - particle.position()(2)) / u_dot_z_normal;
    if (std::isfinite(t) && t >= 0) {
      x = particle.position()(0) + t * particle.momentum()(1);
      y = particle.position()(1) + t * particle.momentum()(2);

      if (x >= m_v0(0) && x <= m_vn(0) && y >= m_v0(1) && y <= m_vn(1)) {
        times.push_back(t);
      }
    }

    // Sort times in increasing order
    std::sort(times.begin(), times.end());

    std::vector<double> unique_times;
    for (double t : times) {
      if (unique_times.empty()) {
        unique_times.push_back(t);
      }
      else {
        if (t != unique_times.back()) {
          unique_times.push_back(t);
        }
      }
    }

    switch (unique_times.size()) {
      case 1:
        return std::make_pair(unique_times[0], std::numeric_limits<double>::quiet_NaN());
      case 2:
        return std::make_pair(unique_times[0], unique_times[1]);
      default:
        return std::make_pair(std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN());
    }
  }

  bool VoxelGrid::transport_particle_to_surface(Particle& particle) const {
    if (this->inside(particle.position())) return false;

    std::pair<double,double> intersection_times = this->intersection_times(particle);
    if (std::isnan(intersection_times.second)) return false;

    particle.position()(0) += intersection_times.first * particle.momentum()(1);
    particle.position()(1) += intersection_times.first * particle.momentum()(2);
    particle.position()(2) += intersection_times.first * particle.momentum()(3);

    return true;
  }
};
