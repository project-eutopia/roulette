#include "roulette/pointwise_three_tensor.h"
#include "roulette/math.h"
#include "roulette/regular_voxel_grid.h"

#include <set>
#include <fstream>
#include <iostream>

namespace roulette {
  PointwiseThreeTensor::PointwiseThreeTensor() :
    m_phantom(),
    m_nx(0),
    m_ny(0),
    m_nz(0),
    m_dummy(0),
    m_points(),
    m_data()
  {
  }

  PointwiseThreeTensor::PointwiseThreeTensor(std::shared_ptr<const Phantom> phantom, const rapidjson::Value& data, double fill_value) :
    m_phantom(phantom),
    m_nx(m_phantom->nx()),
    m_ny(m_phantom->ny()),
    m_nz(m_phantom->nz()),
    m_dummy(0),
    m_points(),
    m_data()
  {
    if (!data.IsObject() || !data.HasMember("points") || !data["points"].IsArray()) {
      throw std::runtime_error("Data for PointwiseThreeTensor must specify points");
    }

    const rapidjson::Value& points = data["points"];

    m_points.reserve(points.Size());

    // Indexes that neighbor a point
    std::set<int> indexes;
    std::tuple<int,int,int> coordinate;
    int xi, yi, zi;
    int cur_index;

    for (int i = 0; i < points.Size(); ++i) {
      m_points.push_back(ThreeVector(points[i]));
      coordinate = m_phantom->voxel_grid()->index_at(m_points.back());

      // Get coordinates of lower left neighbor voxel
      xi = std::get<0>(coordinate);
      yi = std::get<1>(coordinate);
      zi = std::get<2>(coordinate);

      // Check neighbor coordinates
      for (int a = 0; a < 2; ++a) {
        for (int b = 0; b < 2; ++b) {
          for (int c = 0; c < 2; ++c) {
            cur_index = this->internal_index(xi+a, yi+b, zi+c);
            if (cur_index >= 0 && cur_index < this->size()) indexes.insert(cur_index);
          }
        }
      }
    }

    // Touch voxels to trigger storage
    for (int index : indexes) {
      m_data[index] = fill_value;
    }
  }

  int PointwiseThreeTensor::nx() const { return m_nx; }
  int PointwiseThreeTensor::ny() const { return m_ny; }
  int PointwiseThreeTensor::nz() const { return m_nz; }
  int PointwiseThreeTensor::size() const { return m_nx*m_ny*m_nz; }

  double PointwiseThreeTensor::operator()(int xi, int yi, int zi) const {
    auto it = m_data.find(xi + m_nx*yi + (m_nx*m_ny)*zi);

    if (it == m_data.end()) {
      return 0;
    }
    else {
      return it->second;
    }
  }

  double& PointwiseThreeTensor::operator()(int xi, int yi, int zi) {
    int i = xi + m_nx*yi + (m_nx*m_ny)*zi;
    auto it = m_data.find(i);

    if (it == m_data.end()) {
      return m_dummy;
    }
    else {
      return it->second;
    }
  }

  double PointwiseThreeTensor::value_at(const ThreeVector& position) const {
    auto voxel_grid = std::dynamic_pointer_cast<const RegularVoxelGrid>(m_phantom->voxel_grid());
    if (!voxel_grid) throw std::runtime_error("Could not cast voxel grid to RegularVoxelGrid");

    auto normal_coordinates = voxel_grid->normal_coordinates(position);
    return this->trilinearly_interpolated_value(std::get<0>(normal_coordinates), std::get<1>(normal_coordinates), std::get<2>(normal_coordinates));
  }

  std::ofstream& PointwiseThreeTensor::write(std::ofstream& os) const {
    int32_t n;

    n = m_points.size();
    os.write(reinterpret_cast<const char*>(&n), sizeof(n));

    float val;

    for (const auto& point : m_points) {
      val = this->value_at(point);
      os.write(reinterpret_cast<const char*>(&val), sizeof(val));
    }

    return os;
  }

  std::ifstream& PointwiseThreeTensor::read(std::ifstream& is) {
    int32_t n;

    is.read(reinterpret_cast<char*>(&n), sizeof(n));
    m_nx = n;
    is.read(reinterpret_cast<char*>(&n), sizeof(n));
    m_ny = n;
    is.read(reinterpret_cast<char*>(&n), sizeof(n));
    m_nz = n;

    float val;

    m_data = std::map<int,double>();

    while (!is.eof()) {
      is.read(reinterpret_cast<char*>(&n), sizeof(n));
      is.read(reinterpret_cast<char*>(&val), sizeof(val));
      m_data[n] = val;
    }

    return is;
  }

  void PointwiseThreeTensor::rescale(double weight, std::shared_ptr<const ThreeTensor> densities) {
    for (auto& entry : m_data) {
      entry.second *= weight / (*densities)(entry.first % m_nx, (entry.first/m_nx) % m_ny, (entry.first/m_nx/m_ny) % m_nz);
    }
  }

  int PointwiseThreeTensor::internal_index(int xi, int yi, int zi) const {
    return xi + m_nx*yi + (m_nx*m_ny)*zi;
  }
};
