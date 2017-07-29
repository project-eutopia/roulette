#include "roulette/two_tensor.h"

#include <fstream>

namespace roulette {
  TwoTensor::TwoTensor(int nx, int ny, double fill_value) :
    m_nx(nx),
    m_ny(ny),
    m_data(nx*ny, fill_value)
  {
  }

  int TwoTensor::nx() const { return m_nx; }
  int TwoTensor::ny() const { return m_ny; }

  double TwoTensor::operator()(int xi, int yi) const { return m_data[xi + m_nx*yi]; }
  double& TwoTensor::operator()(int xi, int yi) { return m_data[xi + m_nx*yi]; }

  void TwoTensor::write_to_file(std::string filename) const {
    std::ofstream data_file;
    data_file.open(filename, std::ios::out | std::ios::binary);

    // nx, ny
    int32_t n = m_nx;
    data_file.write(reinterpret_cast<char*>(&n), sizeof(int32_t));
    n = m_ny;
    data_file.write(reinterpret_cast<char*>(&n), sizeof(int32_t));

    float val;

    for (int i = 0; i < m_data.size(); ++i) {
      val = (float)m_data[i];
      data_file.write(reinterpret_cast<char*>(&val), sizeof(float));
    }

    data_file.close();
  }
};
