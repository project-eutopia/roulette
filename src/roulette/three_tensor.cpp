#include "roulette/three_tensor.h"

#include <fstream>

namespace roulette {
  ThreeTensor::ThreeTensor() :
    m_nx(0),
    m_ny(0),
    m_nz(0),
    m_data()
  {
  }

  ThreeTensor::ThreeTensor(int nx, int ny, int nz, double fill_value) :
    m_nx(nx),
    m_ny(ny),
    m_nz(nz),
    m_data(nx*ny*nz, fill_value)
  {
  }

  ThreeTensor::ThreeTensor(std::string filename)
  {
    std::ifstream data_file;
    data_file.open(filename, std::ios::in | std::ios::binary);
    this->read(data_file);
    data_file.close();
  }

  int ThreeTensor::nx() const { return m_nx; }
  int ThreeTensor::ny() const { return m_ny; }
  int ThreeTensor::nz() const { return m_nz; }

  double ThreeTensor::operator()(int xi, int yi, int zi) const { return m_data[xi + m_nx*yi + (m_nx*m_ny)*zi]; }
  double& ThreeTensor::operator()(int xi, int yi, int zi) { return m_data[xi + m_nx*yi + (m_nx*m_ny)*zi]; }

  std::ofstream& ThreeTensor::write(std::ofstream& os) const {
    int32_t n;

    n = m_nx;
    os.write(reinterpret_cast<const char*>(&n), sizeof(n));
    n = m_ny;
    os.write(reinterpret_cast<const char*>(&n), sizeof(n));
    n = m_nz;
    os.write(reinterpret_cast<const char*>(&n), sizeof(n));

    float val;
    for (int i = 0; i < m_data.size(); ++i) {
      val = m_data[i];
      os.write(reinterpret_cast<const char*>(&val), sizeof(val));
    }
    return os;
  }

  std::ifstream& ThreeTensor::read(std::ifstream& is) {
    int32_t n;

    is.read(reinterpret_cast<char*>(&n), sizeof(n));
    m_nx = n;
    is.read(reinterpret_cast<char*>(&n), sizeof(n));
    m_ny = n;
    is.read(reinterpret_cast<char*>(&n), sizeof(n));
    m_nz = n;

    float val;

    m_data = std::vector<double>(m_nx*m_ny*m_nz);
    for (int i = 0; i < m_data.size(); ++i) {
      is.read(reinterpret_cast<char*>(&val), sizeof(val));
      m_data[i] = val;
    }

    return is;
  }
};
