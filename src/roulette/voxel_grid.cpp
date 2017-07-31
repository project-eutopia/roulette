#include "roulette/voxel_grid.h"

#include "roulette/particle.h"

#include <cassert>
#include <cmath>

namespace roulette {
  VoxelGrid::VoxelGrid() :
    m_nx(0),
    m_ny(0),
    m_nz(0),
    m_v0(),
    m_vn(),
    m_delta_x(0),
    m_delta_y(0),
    m_delta_z(0)
  {
  }

  VoxelGrid::VoxelGrid(const rapidjson::Value& data) :
    m_nx(data["nx"].GetInt()),
    m_ny(data["ny"].GetInt()),
    m_nz(data["nz"].GetInt()),
    m_v0(data["bottom_left"][0].GetDouble(), data["bottom_left"][1].GetDouble(), data["bottom_left"][2].GetDouble()),
    m_vn(data["top_right"][0].GetDouble(), data["top_right"][1].GetDouble(), data["top_right"][2].GetDouble()),
    m_delta_x((m_vn(0) - m_v0(0)) / m_nx),
    m_delta_y((m_vn(1) - m_v0(1)) / m_ny),
    m_delta_z((m_vn(2) - m_v0(2)) / m_nz)
  {
  }

  VoxelGrid::VoxelGrid(const ThreeVector& v0, const ThreeVector& vn, int nx, int ny, int nz) :
    m_nx(nx),
    m_ny(ny),
    m_nz(nz),
    m_v0(v0),
    m_vn(vn),
    m_delta_x((m_vn(0) - m_v0(0)) / m_nx),
    m_delta_y((m_vn(1) - m_v0(1)) / m_ny),
    m_delta_z((m_vn(2) - m_v0(2)) / m_nz)
  {
  }

  const ThreeVector& VoxelGrid::v0() const { return m_v0; }
  const ThreeVector& VoxelGrid::vn() const { return m_vn; }

  int VoxelGrid::nx() const { return m_nx; }
  int VoxelGrid::ny() const { return m_ny; }
  int VoxelGrid::nz() const { return m_nz; }

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

  bool VoxelGrid::transport_particle_to_surface(Particle* particle) const {
    return this->transport_position_to_surface(particle->position(), particle->momentum().three_momentum());
  }

  bool VoxelGrid::transport_position_to_surface(ThreeVector& position, const ThreeVector& direction) const {
    double u_dot_x_normal = direction(0);
    double u_dot_y_normal = direction(1);
    double u_dot_z_normal = direction(2);

    std::vector<std::pair<int,double>> plane_and_times;

    double t;
    double x, y, z;

    // Low x plane
    t = (m_v0(0) - position(0)) / u_dot_x_normal;
    if (std::isfinite(t) && t >= 0) {
      y = position(1) + t * direction(1);
      z = position(2) + t * direction(2);

      if (y >= m_v0(1) && y <= m_vn(1) && z >= m_v0(2) && z <= m_vn(2)) {
        plane_and_times.push_back(std::make_pair(0, t));
      }
    }

    // High x plane
    t = (m_vn(0) - position(0)) / u_dot_x_normal;
    if (std::isfinite(t) && t >= 0) {
      y = position(1) + t * direction(1);
      z = position(2) + t * direction(2);

      if (y >= m_v0(1) && y <= m_vn(1) && z >= m_v0(2) && z <= m_vn(2)) {
        plane_and_times.push_back(std::make_pair(1, t));
      }
    }

    // Low y plane
    t = (m_v0(1) - position(1)) / u_dot_y_normal;
    if (std::isfinite(t) && t >= 0) {
      x = position(0) + t * direction(0);
      z = position(2) + t * direction(2);

      if (x >= m_v0(0) && x <= m_vn(0) && z >= m_v0(2) && z <= m_vn(2)) {
        plane_and_times.push_back(std::make_pair(2, t));
      }
    }

    // High y plane
    t = (m_vn(1) - position(1)) / u_dot_y_normal;
    if (std::isfinite(t) && t >= 0) {
      x = position(0) + t * direction(0);
      z = position(2) + t * direction(2);

      if (x >= m_v0(0) && x <= m_vn(0) && z >= m_v0(2) && z <= m_vn(2)) {
        plane_and_times.push_back(std::make_pair(3, t));
      }
    }

    // Low z plane
    t = (m_v0(2) - position(2)) / u_dot_z_normal;
    if (std::isfinite(t) && t >= 0) {
      x = position(0) + t * direction(0);
      y = position(1) + t * direction(1);

      if (x >= m_v0(0) && x <= m_vn(0) && y >= m_v0(1) && y <= m_vn(1)) {
        plane_and_times.push_back(std::make_pair(4, t));
      }
    }

    // High z plane
    t = (m_vn(2) - position(2)) / u_dot_z_normal;
    if (std::isfinite(t) && t >= 0) {
      x = position(0) + t * direction(0);
      y = position(1) + t * direction(1);

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

    if (this->outside(position)) {
      // Coming from outside, to pass through must hit TWO surfaces
      if (unique_times.size() != 2) return false;

      double middle_t = (unique_times[0].second + unique_times[1].second) / 2.0;
      ThreeVector middle(
        position(0) + middle_t * direction(0),
        position(1) + middle_t * direction(1),
        position(2) + middle_t * direction(2)
      );

      // If a middle point of the intersecting tangent is not strictly within the bounds,
      // then we are just skimming a surface, so skip
      if (!this->strictly_inside(middle)) return false;
    }

    switch(unique_times[0].first) {
      case 0:
        position(0) = m_v0(0);
        position(1) += unique_times[0].second * direction(1);
        position(2) += unique_times[0].second * direction(2);
        return true;
      case 1:
        position(0) = m_vn(0);
        position(1) += unique_times[0].second * direction(1);
        position(2) += unique_times[0].second * direction(2);
        return true;
      case 2:
        position(0) += unique_times[0].second * direction(0);
        position(1) = m_v0(1);
        position(2) += unique_times[0].second * direction(2);
        return true;
      case 3:
        position(0) += unique_times[0].second * direction(0);
        position(1) = m_vn(1);
        position(2) += unique_times[0].second * direction(2);
        return true;
      case 4:
        position(0) += unique_times[0].second * direction(0);
        position(1) += unique_times[0].second * direction(1);
        position(2) = m_v0(2);
        return true;
      case 5:
        position(0) += unique_times[0].second * direction(0);
        position(1) += unique_times[0].second * direction(1);
        position(2) = m_vn(2);
        return true;
      default:
        assert(false);
        return false;
    }
  }

  double VoxelGrid::exit_time(const ThreeVector& position, const ThreeVector& velocity) const {
    double t = std::numeric_limits<double>::infinity();

    if (velocity(0) < 0) {
      t = std::min(t, (this->v0()(0) - position(0)) / velocity(0));
    }
    else if (velocity(0) > 0) {
      t = std::min(t, (this->vn()(0) - position(0)) / velocity(0));
    }

    if (velocity(1) < 0) {
      t = std::min(t, (this->v0()(1) - position(1)) / velocity(1));
    }
    else if (velocity(1) > 0) {
      t = std::min(t, (this->vn()(1) - position(1)) / velocity(1));
    }

    if (velocity(2) < 0) {
      t = std::min(t, (this->v0()(2) - position(2)) / velocity(2));
    }
    else if (velocity(2) > 0) {
      t = std::min(t, (this->vn()(2) - position(2)) / velocity(2));
    }

    return t;
  }

  std::ofstream& VoxelGrid::write(std::ofstream& os) const {
    int32_t n;

    n = m_nx;
    os.write(reinterpret_cast<const char*>(&n), sizeof(n));
    n = m_ny;
    os.write(reinterpret_cast<const char*>(&n), sizeof(n));
    n = m_nz;
    os.write(reinterpret_cast<const char*>(&n), sizeof(n));

    m_v0.write(os);
    m_vn.write(os);
    return os;
  }

  std::ifstream& VoxelGrid::read(std::ifstream& is) {
    int32_t n;

    is.read(reinterpret_cast<char*>(&n), sizeof(n));
    n = m_nx;
    is.read(reinterpret_cast<char*>(&n), sizeof(n));
    n = m_ny;
    is.read(reinterpret_cast<char*>(&n), sizeof(n));
    n = m_nz;

    m_v0.read(is);
    m_vn.read(is);

    m_delta_x = (m_vn(0) - m_v0(0)) / m_nx;
    m_delta_y = (m_vn(1) - m_v0(1)) / m_ny;
    m_delta_z = (m_vn(2) - m_v0(2)) / m_nz;
    return is;
  }
};
