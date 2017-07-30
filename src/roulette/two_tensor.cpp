#include "roulette/two_tensor.h"

#include <fstream>

namespace roulette {
  TwoTensor::TwoTensor() :
    m_nx(0),
    m_ny(0),
    m_data()
  {
  }

  TwoTensor::TwoTensor(const rapidjson::Value& matrix) :
    m_nx(matrix[0].Size()),
    m_ny(matrix.Size()),
    m_data(m_nx*m_ny, 0)
  {
    m_ny = matrix.Size();
    m_nx = matrix[0].Size();

    for (int yi = 0; yi < m_ny; ++yi) {
      assert(matrix[yi].Size() == m_nx);
      for (int xi = 0; xi < m_nx; ++xi) {
        (*this)(xi, yi) = matrix[yi][xi].GetDouble();
      }
    }
  }

  TwoTensor::TwoTensor(int nx, int ny, double fill_value) :
    m_nx(nx),
    m_ny(ny),
    m_data(m_nx*m_ny, fill_value)
  {
  }

  int TwoTensor::nx() const { return m_nx; }
  int TwoTensor::ny() const { return m_ny; }

  double TwoTensor::operator()(int xi, int yi) const { return m_data[xi + m_nx*yi]; }
  double& TwoTensor::operator()(int xi, int yi) { return m_data[xi + m_nx*yi]; }

  std::ofstream& TwoTensor::write(std::ofstream& os) const {
    // TODO: fix size of data exactly for portability
    os.write(reinterpret_cast<const char*>(&m_nx), sizeof(m_nx));
    os.write(reinterpret_cast<const char*>(&m_ny), sizeof(m_ny));
    os.write(reinterpret_cast<const char*>(m_data.data()), m_data.size() * sizeof(decltype(m_data)::value_type));
    return os;
  }

  std::ifstream& TwoTensor::read(std::ifstream& is) {
    is.read(reinterpret_cast<char*>(&m_nx), sizeof(m_nx));
    is.read(reinterpret_cast<char*>(&m_ny), sizeof(m_ny));

    m_data = std::vector<double>(m_nx*m_ny);
    is.read(reinterpret_cast<char*>(m_data.data()), m_data.size() * sizeof(decltype(m_data)::value_type));
    return is;
  }
};
