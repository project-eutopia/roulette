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

  int ThreeTensor::nx() const { return m_nx; }
  int ThreeTensor::ny() const { return m_ny; }
  int ThreeTensor::nz() const { return m_nz; }

  double ThreeTensor::operator()(int xi, int yi, int zi) const { return m_data[xi + m_nx*yi + (m_nx*m_ny)*zi]; }
  double& ThreeTensor::operator()(int xi, int yi, int zi) { return m_data[xi + m_nx*yi + (m_nx*m_ny)*zi]; }

  void ThreeTensor::write_to_file(std::string filename) const {
    std::ofstream data_file;
    data_file.open(filename, std::ios::out | std::ios::binary);

    // nx, ny, nz
    int32_t n = m_nx;
    data_file.write(reinterpret_cast<char*>(&n), sizeof(int32_t));
    n = m_ny;
    data_file.write(reinterpret_cast<char*>(&n), sizeof(int32_t));
    n = m_nz;
    data_file.write(reinterpret_cast<char*>(&n), sizeof(int32_t));

    float val;

    for (int i = 0; i < m_data.size(); ++i) {
      val = (float)m_data[i];
      data_file.write(reinterpret_cast<char*>(&val), sizeof(float));
    }

    data_file.close();
  }
};
