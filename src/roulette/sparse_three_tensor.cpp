#include "roulette/sparse_three_tensor.h"

#include <fstream>
#include <iostream>

namespace roulette {
  SparseThreeTensor::SparseThreeTensor() :
    m_nx(0),
    m_ny(0),
    m_nz(0),
    m_data()
  {
  }

  SparseThreeTensor::SparseThreeTensor(int nx, int ny, int nz, double fill_value) :
    m_nx(nx),
    m_ny(ny),
    m_nz(nz),
    m_default(fill_value),
    m_data()
  {
  }

  SparseThreeTensor::SparseThreeTensor(std::string filename)
  {
    std::ifstream data_file;
    data_file.open(filename, std::ios::in | std::ios::binary);
    this->read(data_file);
    data_file.close();
  }

  int SparseThreeTensor::nx() const { return m_nx; }
  int SparseThreeTensor::ny() const { return m_ny; }
  int SparseThreeTensor::nz() const { return m_nz; }

  double SparseThreeTensor::operator()(int xi, int yi, int zi) const {
    auto it = m_data.find(this->internal_index(xi, yi, zi));

    if (it == m_data.end()) {
      return m_default;
    }
    else {
      return it->second;
    }
  }

  double& SparseThreeTensor::operator()(int xi, int yi, int zi) {
    int i = this->internal_index(xi, yi, zi);
    auto it = m_data.find(i);

    if (it == m_data.end()) {
      m_data[i] = m_default;
      return m_data[i];
    }
    else {
      return it->second;
    }
  }

  void SparseThreeTensor::set(int xi, int yi, int zi, double value) {
    std::lock_guard<std::mutex> guard(m_data_mutex);
    (*this)(xi, yi, zi) = value;
  }

  void SparseThreeTensor::increment(int xi, int yi, int zi, double delta) {
    std::lock_guard<std::mutex> guard(m_data_mutex);
    (*this)(xi, yi, zi) += delta;
  }

  std::ofstream& SparseThreeTensor::write(std::ofstream& os) const {
    int32_t n;

    n = m_nx;
    os.write(reinterpret_cast<const char*>(&n), sizeof(n));
    n = m_ny;
    os.write(reinterpret_cast<const char*>(&n), sizeof(n));
    n = m_nz;
    os.write(reinterpret_cast<const char*>(&n), sizeof(n));

    float val;
    val = m_default;
    os.write(reinterpret_cast<const char*>(&val), sizeof(val));

    for (auto entry : m_data) {
      n = entry.first;
      val = entry.second;
      os.write(reinterpret_cast<const char*>(&n), sizeof(n));
      os.write(reinterpret_cast<const char*>(&val), sizeof(val));
    }

    return os;
  }

  std::ifstream& SparseThreeTensor::read(std::ifstream& is) {
    int32_t n;

    is.read(reinterpret_cast<char*>(&n), sizeof(n));
    m_nx = n;
    is.read(reinterpret_cast<char*>(&n), sizeof(n));
    m_ny = n;
    is.read(reinterpret_cast<char*>(&n), sizeof(n));
    m_nz = n;

    float val;
    is.read(reinterpret_cast<char*>(&val), sizeof(val));
    m_default = val;

    m_data = std::map<int,double>();

    while (!is.eof()) {
      is.read(reinterpret_cast<char*>(&n), sizeof(n));
      is.read(reinterpret_cast<char*>(&val), sizeof(val));
      m_data[n] = val;
    }

    return is;
  }

  void SparseThreeTensor::rescale(double weight, std::shared_ptr<const ThreeTensor> densities) {
    for (auto& entry : m_data) {
      entry.second *= weight / (*densities)(entry.first % m_nx, (entry.first/m_nx) % m_ny, (entry.first/m_nx/m_ny) % m_nz);
    }
  }

  int SparseThreeTensor::internal_index(int xi, int yi, int zi) const {
    return xi + m_nx*yi + (m_nx*m_ny)*zi;
  }
};
