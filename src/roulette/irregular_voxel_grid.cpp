#include "roulette/irregular_voxel_grid.h"

#include "roulette/particle.h"
#include "roulette/phantom.h"

#include <algorithm>
#include <cassert>
#include <cmath>

namespace roulette {
  IrregularVoxelGrid::IrregularVoxelGrid() :
    m_xplanes(),
    m_yplanes(),
    m_zplanes(),
    m_nx(0),
    m_ny(0),
    m_nz(0),
    m_v0(),
    m_vn()
  {
  }

  IrregularVoxelGrid::IrregularVoxelGrid(const rapidjson::Value& data) : IrregularVoxelGrid()
  {
    if (!data.HasMember("xplanes") || !data["xplanes"].IsArray()) {
      throw InvalidVoxelGrid("IrregularVoxelGrid requires \"xplanes\"");
    }

    m_xplanes.reserve(data["xplanes"].Size());
    for (size_t i = 0; i < data["xplanes"].Size(); ++i) {
      m_xplanes.push_back(data["xplanes"][0].GetDouble());
    }

    if (!data.HasMember("yplanes") || !data["yplanes"].IsArray()) {
      throw InvalidVoxelGrid("IrregularVoxelGrid requires \"yplanes\"");
    }

    m_yplanes.reserve(data["yplanes"].Size());
    for (size_t i = 0; i < data["yplanes"].Size(); ++i) {
      m_yplanes.push_back(data["yplanes"][0].GetDouble());
    }

    if (!data.HasMember("zplanes") || !data["zplanes"].IsArray()) {
      throw InvalidVoxelGrid("IrregularVoxelGrid requires \"zplanes\"");
    }

    m_zplanes.reserve(data["zplanes"].Size());
    for (size_t i = 0; i < data["zplanes"].Size(); ++i) {
      m_zplanes.push_back(data["zplanes"][0].GetDouble());
    }

    m_nx = m_xplanes.size()-1;
    m_ny = m_yplanes.size()-1;
    m_nz = m_zplanes.size()-1;
    m_v0 = ThreeVector(m_xplanes.front(), m_yplanes.front(), m_zplanes.front());
    m_vn = ThreeVector(m_xplanes.back(), m_yplanes.back(), m_zplanes.back());

    this->validate_planes();
  }

  IrregularVoxelGrid::IrregularVoxelGrid(const std::vector<double>& xplanes, const std::vector<double>& yplanes, const std::vector<double>& zplanes) :
    m_xplanes(xplanes),
    m_yplanes(yplanes),
    m_zplanes(zplanes),
    m_nx(m_xplanes.size()-1),
    m_ny(m_yplanes.size()-1),
    m_nz(m_zplanes.size()-1),
    m_v0(m_xplanes.front(), m_yplanes.front(), m_zplanes.front()),
    m_vn(m_xplanes.back(), m_yplanes.back(), m_zplanes.back())
  {
    this->validate_planes();
  }

  const ThreeVector& IrregularVoxelGrid::v0() const { return m_v0; }
  const ThreeVector& IrregularVoxelGrid::vn() const { return m_vn; }

  size_t IrregularVoxelGrid::nx() const { return m_nx; }
  size_t IrregularVoxelGrid::ny() const { return m_ny; }
  size_t IrregularVoxelGrid::nz() const { return m_nz; }

  std::tuple<size_t,size_t,size_t> IrregularVoxelGrid::index_at(const ThreeVector& position) const {
    return std::make_tuple(
      (size_t)(std::upper_bound(m_xplanes.begin(), m_xplanes.end(), position(0)) - m_xplanes.begin() - 1),
      (size_t)(std::upper_bound(m_yplanes.begin(), m_yplanes.end(), position(1)) - m_yplanes.begin() - 1),
      (size_t)(std::upper_bound(m_zplanes.begin(), m_zplanes.end(), position(2)) - m_zplanes.begin() - 1)
    );
  }

  std::ofstream& IrregularVoxelGrid::write(std::ofstream& os) const {
    int32_t n;
    float f;

    n = m_nx;
    os.write(reinterpret_cast<const char*>(&n), sizeof(n));
    for (auto x : m_xplanes) {
      f = x;
      os.write(reinterpret_cast<const char*>(&f), sizeof(f));
    }

    n = m_ny;
    os.write(reinterpret_cast<const char*>(&n), sizeof(n));
    for (auto y : m_yplanes) {
      f = y;
      os.write(reinterpret_cast<const char*>(&f), sizeof(f));
    }

    n = m_nz;
    os.write(reinterpret_cast<const char*>(&n), sizeof(n));
    for (auto z : m_zplanes) {
      f = z;
      os.write(reinterpret_cast<const char*>(&f), sizeof(f));
    }

    return os;
  }

  std::ifstream& IrregularVoxelGrid::read(std::ifstream& is) {
    int32_t n;
    float f;

    is.read(reinterpret_cast<char*>(&n), sizeof(n));
    n = m_nx;
    m_xplanes.reserve(m_nx+1);

    for (size_t i = 0; i <= m_nx; ++i) {
      is.read(reinterpret_cast<char*>(&f), sizeof(f));
      m_xplanes.push_back(f);
    }

    is.read(reinterpret_cast<char*>(&n), sizeof(n));
    n = m_ny;
    m_yplanes.reserve(m_ny+1);

    for (size_t i = 0; i <= m_ny; ++i) {
      is.read(reinterpret_cast<char*>(&f), sizeof(f));
      m_yplanes.push_back(f);
    }

    is.read(reinterpret_cast<char*>(&n), sizeof(n));
    n = m_nz;
    m_zplanes.reserve(m_nz+1);

    for (size_t i = 0; i <= m_nz; ++i) {
      is.read(reinterpret_cast<char*>(&f), sizeof(f));
      m_zplanes.push_back(f);
    }

    m_v0 = ThreeVector(m_xplanes.front(), m_yplanes.front(), m_zplanes.front());
    m_vn = ThreeVector(m_xplanes.back(), m_yplanes.back(), m_zplanes.back());

    return is;
  }

  rapidjson::Value IrregularVoxelGrid::to_json(rapidjson::Document::AllocatorType& allocator) const {
    rapidjson::Value v;
    v.SetObject();

    v.AddMember("type", rapidjson::Value().SetString("IrregularVoxelGrid"), allocator);

    {
      rapidjson::Value a;
      a.SetArray();

      for (auto x : m_xplanes) {
        a.PushBack(rapidjson::Value().SetDouble(x), allocator);
      }

      v.AddMember("xplanes", a, allocator);
    }

    {
      rapidjson::Value a;
      a.SetArray();

      for (auto y : m_yplanes) {
        a.PushBack(rapidjson::Value().SetDouble(y), allocator);
      }

      v.AddMember("yplanes", a, allocator);
    }

    {
      rapidjson::Value a;
      a.SetArray();

      for (auto z : m_zplanes) {
        a.PushBack(rapidjson::Value().SetDouble(z), allocator);
      }

      v.AddMember("zplanes", a, allocator);
    }

    return v;
  }

  ThreeVector IrregularVoxelGrid::ray_trace_voxels(const ThreeVector& initial_position, const ThreeVector& direction, IVoxelGrid::voxel_iterator it) const {
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

    // Set increments to +1 for moving forward, -1 for backward, and 0 for stationary
    //
    // Also, the initial voxel index depends on the direction of travel as follows:
    // Moving forward, voxel i has normal coordinate range [i, i+1) (i.e. floor(x))
    // Moving backward, voxel i has normal coordinate range (i, i+1] (i.e. ceil(x-1))
    if (u(0) < 0) {
      xinc = -1;
      xi = std::lower_bound(m_xplanes.begin(), m_xplanes.end(), current_position(0)) - m_xplanes.begin() - 1;
    }
    else if (u(0) > 0) {
      xinc = 1;
      xi = std::upper_bound(m_xplanes.begin(), m_xplanes.end(), current_position(0)) - m_xplanes.begin() - 1;
    }
    else {
      xinc = 0;
      xi = std::upper_bound(m_xplanes.begin(), m_xplanes.end(), current_position(0)) - m_xplanes.begin() - 1;
    }

    if (u(1) < 0) {
      yinc = -1;
      yi = std::lower_bound(m_yplanes.begin(), m_yplanes.end(), current_position(1)) - m_yplanes.begin() - 1;
    }
    else if (u(1) > 0) {
      yinc = 1;
      yi = std::upper_bound(m_yplanes.begin(), m_yplanes.end(), current_position(1)) - m_yplanes.begin() - 1;
    }
    else {
      yinc = 0;
      yi = std::upper_bound(m_yplanes.begin(), m_yplanes.end(), current_position(1)) - m_yplanes.begin() - 1;
    }

    if (u(2) < 0) {
      zinc = -1;
      zi = std::lower_bound(m_zplanes.begin(), m_zplanes.end(), current_position(2)) - m_zplanes.begin() - 1;
    }
    else if (u(2) > 0) {
      zinc = 1;
      zi = std::upper_bound(m_zplanes.begin(), m_zplanes.end(), current_position(2)) - m_zplanes.begin() - 1;
    }
    else {
      zinc = 0;
      zi = std::upper_bound(m_zplanes.begin(), m_zplanes.end(), current_position(2)) - m_zplanes.begin() - 1;
    }

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

    double delta_t = 0;

    // If not incrementing, permanently set time to next voxel along that coordinate to
    // infinity so it is never considered
    double time_to_x;
    double time_to_y;
    double time_to_z;

    if (xinc == 0) time_to_x = std::numeric_limits<double>::infinity();
    if (yinc == 0) time_to_y = std::numeric_limits<double>::infinity();
    if (zinc == 0) time_to_z = std::numeric_limits<double>::infinity();

    while (xi_factor*xi + xi_offset >= 0 && yi_factor*yi + yi_offset >= 0 && zi_factor*zi + zi_offset >= 0) {
      if (xinc) time_to_x = std::abs((m_xplanes[xi + (u(0)>0)] - current_position(0)) / u(0));
      if (yinc) time_to_y = std::abs((m_yplanes[yi + (u(1)>0)] - current_position(1)) / u(1));
      if (zinc) time_to_z = std::abs((m_zplanes[zi + (u(2)>0)] - current_position(2)) / u(2));

      delta_t = std::min(time_to_x, std::min(time_to_y, time_to_z));

      // Iterator callback
      double distance = it(delta_t, xi, yi, zi);
      // Here we are finished, so return final position
      if (distance < delta_t) return current_position + distance * u;

      // If passing through an edge or corner, will increment multiple indexes here (happens when, e.g., tx = ty < tz)
      if (time_to_x <= time_to_y && time_to_x <= time_to_z) {
        xi += xinc;
      }

      if (time_to_y <= time_to_x && time_to_y <= time_to_z) {
        yi += yinc;
      }

      if (time_to_z <= time_to_x && time_to_z <= time_to_y) {
        zi += zinc;
      }

      current_position += delta_t * u;
    }

    return current_position;
  }

  void IrregularVoxelGrid::validate_planes() const {
    if (
      !std::is_sorted(m_xplanes.begin(), m_xplanes.end()) ||
      !std::is_sorted(m_yplanes.begin(), m_yplanes.end()) ||
      !std::is_sorted(m_zplanes.begin(), m_zplanes.end())
    )
    {
      throw InvalidVoxelGrid("Planes must be in increasing order");
    }
  }
};
