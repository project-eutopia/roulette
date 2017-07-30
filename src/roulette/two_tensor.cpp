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

  std::ofstream& TwoTensor::write(std::ofstream& os) const {
    os.write(reinterpret_cast<const char*>(&m_nx), sizeof(int));
    os.write(reinterpret_cast<const char*>(&m_ny), sizeof(int));
    os.write(reinterpret_cast<const char*>(m_data.data()), m_data.size() * sizeof(double));
    return os;
  }

  std::ifstream& TwoTensor::read(std::ifstream& is) {
    is.read(reinterpret_cast<char*>(&m_nx), sizeof(int));
    is.read(reinterpret_cast<char*>(&m_ny), sizeof(int));

    m_data = std::vector<double>(m_nx*m_ny);
    is.read(reinterpret_cast<char*>(m_data.data()), m_data.size() * sizeof(double));
    return is;
  }
};
