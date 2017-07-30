#include "roulette/three_tensor.h"

#include <fstream>

namespace roulette {
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

    int32_t n;
    data_file.read(reinterpret_cast<char*>(&n), sizeof(int32_t));
    m_nx = (int)n;
    data_file.read(reinterpret_cast<char*>(&n), sizeof(int32_t));
    m_ny = (int)n;
    data_file.read(reinterpret_cast<char*>(&n), sizeof(int32_t));
    m_nz = (int)n;

    int N = m_nx * m_ny * m_nz;

    float val;
    m_data.reserve(N);

    for (int i = 0; i < N; ++i) {
      data_file.read(reinterpret_cast<char*>(&val), sizeof(float));
      m_data.push_back((double)val);
    }
  }

  int ThreeTensor::nx() const { return m_nx; }
  int ThreeTensor::ny() const { return m_ny; }
  int ThreeTensor::nz() const { return m_nz; }

  double ThreeTensor::operator()(int xi, int yi, int zi) const { return m_data[xi + m_nx*yi + (m_nx*m_ny)*zi]; }
  double& ThreeTensor::operator()(int xi, int yi, int zi) { return m_data[xi + m_nx*yi + (m_nx*m_ny)*zi]; }

  std::ofstream& ThreeTensor::write(std::ofstream& os) const {
    os.write(reinterpret_cast<const char*>(&m_nx), sizeof(int));
    os.write(reinterpret_cast<const char*>(&m_ny), sizeof(int));
    os.write(reinterpret_cast<const char*>(&m_nz), sizeof(int));
    os.write(reinterpret_cast<const char*>(m_data.data()), m_data.size() * sizeof(double));
    return os;
  }

  std::ifstream& ThreeTensor::read(std::ifstream& is) {
    is.read(reinterpret_cast<char*>(&m_nx), sizeof(int));
    is.read(reinterpret_cast<char*>(&m_ny), sizeof(int));
    is.read(reinterpret_cast<char*>(&m_nz), sizeof(int));

    m_data = std::vector<double>(m_nx*m_ny*m_nz);
    is.read(reinterpret_cast<char*>(m_data.data()), m_data.size() * sizeof(double));
    return is;
  }
};
