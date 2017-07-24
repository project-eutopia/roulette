#include "roulette/voxel_grid.h"

#include <cassert>
#include <cmath>

namespace roulette {
  VoxelGrid::VoxelGrid(const ThreeVector& v0, const ThreeVector& vn) :
    m_v0(v0),
    m_vn(vn)
  {
  }

  const ThreeVector& VoxelGrid::v0() const { return m_v0; }
  const ThreeVector& VoxelGrid::vn() const { return m_vn; }

  double VoxelGrid::delta_x() const { return m_delta_x; }
  double VoxelGrid::delta_y() const { return m_delta_y; }
  double VoxelGrid::delta_z() const { return m_delta_z; }

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

  bool VoxelGrid::transport_particle_to_surface(Particle& particle) const {
    double u_dot_x_normal = particle.momentum()(1);
    double u_dot_y_normal = particle.momentum()(2);
    double u_dot_z_normal = particle.momentum()(3);

    std::vector<std::pair<int,double>> plane_and_times;

    double t;
    double x, y, z;

    // Low x plane
    t = (m_v0(0) - particle.position()(0)) / u_dot_x_normal;
    if (std::isfinite(t) && t >= 0) {
      y = particle.position()(1) + t * particle.momentum()(2);
      z = particle.position()(2) + t * particle.momentum()(3);

      if (y >= m_v0(1) && y <= m_vn(1) && z >= m_v0(2) && z <= m_vn(2)) {
        plane_and_times.push_back(std::make_pair(0, t));
      }
    }

    // High x plane
    t = (m_vn(0) - particle.position()(0)) / u_dot_x_normal;
    if (std::isfinite(t) && t >= 0) {
      y = particle.position()(1) + t * particle.momentum()(2);
      z = particle.position()(2) + t * particle.momentum()(3);

      if (y >= m_v0(1) && y <= m_vn(1) && z >= m_v0(2) && z <= m_vn(2)) {
        plane_and_times.push_back(std::make_pair(1, t));
      }
    }

    // Low y plane
    t = (m_v0(1) - particle.position()(1)) / u_dot_y_normal;
    if (std::isfinite(t) && t >= 0) {
      x = particle.position()(0) + t * particle.momentum()(1);
      z = particle.position()(2) + t * particle.momentum()(3);

      if (x >= m_v0(0) && x <= m_vn(0) && z >= m_v0(2) && z <= m_vn(2)) {
        plane_and_times.push_back(std::make_pair(2, t));
      }
    }

    // High y plane
    t = (m_vn(1) - particle.position()(1)) / u_dot_y_normal;
    if (std::isfinite(t) && t >= 0) {
      x = particle.position()(0) + t * particle.momentum()(1);
      z = particle.position()(2) + t * particle.momentum()(3);

      if (x >= m_v0(0) && x <= m_vn(0) && z >= m_v0(2) && z <= m_vn(2)) {
        plane_and_times.push_back(std::make_pair(3, t));
      }
    }

    // Low z plane
    t = (m_v0(2) - particle.position()(2)) / u_dot_z_normal;
    if (std::isfinite(t) && t >= 0) {
      x = particle.position()(0) + t * particle.momentum()(1);
      y = particle.position()(1) + t * particle.momentum()(2);

      if (x >= m_v0(0) && x <= m_vn(0) && y >= m_v0(1) && y <= m_vn(1)) {
        plane_and_times.push_back(std::make_pair(4, t));
      }
    }

    // High z plane
    t = (m_vn(2) - particle.position()(2)) / u_dot_z_normal;
    if (std::isfinite(t) && t >= 0) {
      x = particle.position()(0) + t * particle.momentum()(1);
      y = particle.position()(1) + t * particle.momentum()(2);

      if (x >= m_v0(0) && x <= m_vn(0) && y >= m_v0(1) && y <= m_vn(1)) {
        plane_and_times.push_back(std::make_pair(5, t));
      }
    }

    // Sort times in increasing order
    std::sort(plane_and_times.begin(), plane_and_times.end(), [](auto &left, auto &right) {
      return left.second < right.second;
    });

    std::vector<std::pair<int,double>> unique_times;
    for (const auto& pt : plane_and_times) {
      if (unique_times.empty()) {
        unique_times.push_back(pt);
      }
      else {
        if (pt.second != unique_times.back().second) {
          unique_times.push_back(pt);
        }
      }
    }

    // If do not intersect, cannot transport to surface
    if (unique_times.size() == 0) return false;

    if (this->outside(particle.position())) {
      // Coming from outside, to pass through must hit TWO surfaces
      if (unique_times.size() != 2) return false;

      double middle_t = (unique_times[0].second + unique_times[1].second) / 2.0;
      ThreeVector middle(
        particle.position()(0) + middle_t * particle.momentum()(1),
        particle.position()(1) + middle_t * particle.momentum()(2),
        particle.position()(2) + middle_t * particle.momentum()(3)
      );

      // If a middle point of the intersecting tangent is not strictly within the bounds,
      // then we are just skimming a surface, so skip
      if (!this->strictly_inside(middle)) return false;
    }

    switch(unique_times[0].first) {
      case 0:
        particle.position()(0) = m_v0(0);
        particle.position()(1) += unique_times[0].second * particle.momentum()(2);
        particle.position()(2) += unique_times[0].second * particle.momentum()(3);
        return true;
      case 1:
        particle.position()(0) = m_vn(0);
        particle.position()(1) += unique_times[0].second * particle.momentum()(2);
        particle.position()(2) += unique_times[0].second * particle.momentum()(3);
        return true;
      case 2:
        particle.position()(0) += unique_times[0].second * particle.momentum()(1);
        particle.position()(1) = m_v0(1);
        particle.position()(2) += unique_times[0].second * particle.momentum()(3);
        return true;
      case 3:
        particle.position()(0) += unique_times[0].second * particle.momentum()(1);
        particle.position()(1) = m_vn(1);
        particle.position()(2) += unique_times[0].second * particle.momentum()(3);
        return true;
      case 4:
        particle.position()(0) += unique_times[0].second * particle.momentum()(1);
        particle.position()(1) += unique_times[0].second * particle.momentum()(2);
        particle.position()(2) = m_v0(2);
        return true;
      case 5:
        particle.position()(0) += unique_times[0].second * particle.momentum()(1);
        particle.position()(1) += unique_times[0].second * particle.momentum()(2);
        particle.position()(2) = m_vn(2);
        return true;
      default:
        assert(false);
        return false;
    }
  }
};
