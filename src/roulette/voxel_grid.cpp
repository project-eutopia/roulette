#include "roulette/voxel_grid.h"

#include "roulette/particle.h"
#include "roulette/phantom.h"

#include <algorithm>
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

  std::tuple<int,int,int> VoxelGrid::index_at(const ThreeVector& position) const {
    auto normal = this->normal_coordinates(position);
    return std::make_tuple((int)std::get<0>(normal), (int)std::get<1>(normal), (int)std::get<2>(normal));
  }

  std::tuple<double,double,double> VoxelGrid::normal_coordinates(const ThreeVector& position) const {
    return std::make_tuple(
      (position(0) - m_v0(0)) / m_delta_x,
      (position(1) - m_v0(1)) / m_delta_y,
      (position(2) - m_v0(2)) / m_delta_z
    );
  }

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

  rapidjson::Value VoxelGrid::to_json(rapidjson::Document::AllocatorType& allocator) const {
    rapidjson::Value v;
    v.SetObject();

    v.AddMember("nx", rapidjson::Value().SetInt(m_nx), allocator);
    v.AddMember("ny", rapidjson::Value().SetInt(m_ny), allocator);
    v.AddMember("nz", rapidjson::Value().SetInt(m_nz), allocator);

    v.AddMember("v0", m_v0.to_json(allocator), allocator);
    v.AddMember("vn", m_vn.to_json(allocator), allocator);

    return v;
  }

  ThreeVector VoxelGrid::ray_trace_voxels(const ThreeVector& initial_position, const ThreeVector& direction, VoxelGrid::voxel_iterator it) const {
    double mag = direction.magnitude();
    assert(mag > 0);
    ThreeVector u = direction / mag;

    ThreeVector current_position = initial_position;
    // Done if does not intersect surface
    if (this->outside(current_position) && !this->transport_position_to_surface(current_position, u)) {
      return initial_position;
    }

    int xinc, yinc, zinc;
    int xi, yi, zi;

    // Coordinates in units of voxel indexes
    auto normal = this->normal_coordinates(current_position);

    // Set increments to +1 for moving forward, -1 for backward, and 0 for stationary
    //
    // Also, the initial voxel index depends on the direction of travel as follows:
    // Moving forward, voxel i has normal coordinate range [i, i+1) (i.e. floor(x))
    // Moving backward, voxel i has normal coordinate range (i, i+1] (i.e. ceil(x-1))
    if (u(0) < 0) {
      xinc = -1;
      xi = math::ceili(std::get<0>(normal)-1);
    }
    else if (u(0) > 0) {
      xinc = 1;
      xi = math::floori(std::get<0>(normal));
    }
    else {
      xinc = 0;
      xi = math::floori(std::get<0>(normal));
    }

    if (u(1) < 0) {
      yinc = -1;
      yi = math::ceili(std::get<1>(normal)-1);
    }
    else if (u(1) > 0) {
      yinc = 1;
      yi = math::floori(std::get<1>(normal));
    }
    else {
      yinc = 0;
      yi = math::floori(std::get<1>(normal));
    }

    if (u(2) < 0) {
      zinc = -1;
      zi = math::ceili(std::get<2>(normal)-1);
    }
    else if (u(2) > 0) {
      zinc = 1;
      zi = math::floori(std::get<2>(normal));
    }
    else {
      zinc = 0;
      zi = math::floori(std::get<2>(normal));
    }

    double delta_t = 0;

    double time_between_x_planes = m_delta_x / std::abs(u(0));
    double time_between_y_planes = m_delta_y / std::abs(u(1));
    double time_between_z_planes = m_delta_z / std::abs(u(2));

    // If not incrementing, permanently set time to next voxel along that coordinate to
    // infinity so it is never considered
    double time_to_x = (xinc == 0) ? std::numeric_limits<double>::infinity() : (m_v0(0) + (xi + (xinc > 0)) * m_delta_x - current_position(0)) / u(0);
    double time_to_y = (yinc == 0) ? std::numeric_limits<double>::infinity() : (m_v0(1) + (yi + (yinc > 0)) * m_delta_y - current_position(1)) / u(1);
    double time_to_z = (zinc == 0) ? std::numeric_limits<double>::infinity() : (m_v0(2) + (zi + (zinc > 0)) * m_delta_z - current_position(2)) / u(2);

    // We know the direction of motion, so only need to check one side:
    // Moving left:   xi >= 0
    //
    // Moving right:  xi < nx
    //                xi - nx < 0
    //                xi - nx + 1 <= 0
    //                nx - xi - 1 >= 0
    //
    // Combine into single:
    // xi_factor * xi + xi_offset >= 0
    int xi_factor = (xinc > 0) ? -1 : 1;
    int xi_offset = (xinc > 0) ? this->nx() - 1 : 0;

    int yi_factor = (yinc > 0) ? -1 : 1;
    int yi_offset = (yinc > 0) ? this->ny() - 1 : 0;

    int zi_factor = (zinc > 0) ? -1 : 1;
    int zi_offset = (zinc > 0) ? this->nz() - 1 : 0;

    double total_time = 0;

    while (xi_factor*xi + xi_offset >= 0 && yi_factor*yi + yi_offset >= 0 && zi_factor*zi + zi_offset >= 0) {
      delta_t = std::min(time_to_x, std::min(time_to_y, time_to_z));

      // Iterator callback
      double distance = it(delta_t, xi, yi, zi);
      // Here we are finished, so return final position
      if (distance < delta_t) return current_position + (total_time + distance) * u;

      // If passing through an edge or corner, will increment multiple indexes here (happens when, e.g., tx = ty < tz)
      if (time_to_x <= time_to_y && time_to_x <= time_to_z) {
        time_to_x += time_between_x_planes - delta_t;
        xi += xinc;
      } else {
        time_to_x -= delta_t;
      }

      if (time_to_y <= time_to_x && time_to_y <= time_to_z) {
        time_to_y += time_between_y_planes - delta_t;
        yi += yinc;
      } else {
        time_to_y -= delta_t;
      }

      if (time_to_z <= time_to_x && time_to_z <= time_to_y) {
        time_to_z += time_between_z_planes - delta_t;
        zi += zinc;
      } else {
        time_to_z -= delta_t;
      }

      total_time += delta_t;
    }

    return current_position + total_time * u;
  }
};
