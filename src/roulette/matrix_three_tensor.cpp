#include "roulette/matrix_three_tensor.h"

#include <fstream>

namespace roulette {
  MatrixThreeTensor::MatrixThreeTensor() :
    m_nx(0),
    m_ny(0),
    m_nz(0),
    m_data()
  {
  }

  MatrixThreeTensor::MatrixThreeTensor(int nx, int ny, int nz, double fill_value) :
    m_nx(nx),
    m_ny(ny),
    m_nz(nz),
    m_data(nx*ny*nz, fill_value)
  {
  }

  MatrixThreeTensor::MatrixThreeTensor(std::string filename)
  {
    std::ifstream data_file;
    data_file.open(filename, std::ios::in | std::ios::binary);
    this->read(data_file);
    data_file.close();
  }

  int MatrixThreeTensor::nx() const { return m_nx; }
  int MatrixThreeTensor::ny() const { return m_ny; }
  int MatrixThreeTensor::nz() const { return m_nz; }

  double MatrixThreeTensor::operator()(int xi, int yi, int zi) const { return m_data[xi + m_nx*yi + (m_nx*m_ny)*zi]; }
  double& MatrixThreeTensor::operator()(int xi, int yi, int zi) { return m_data[xi + m_nx*yi + (m_nx*m_ny)*zi]; }

  void MatrixThreeTensor::set(int xi, int yi, int zi, double value) {
    std::lock_guard<std::mutex> guard(m_data_mutex);
    (*this)(xi, yi, zi) = value;
  }

  void MatrixThreeTensor::increment(int xi, int yi, int zi, double delta) {
    std::lock_guard<std::mutex> guard(m_data_mutex);
    (*this)(xi, yi, zi) += delta;
  }

  std::ofstream& MatrixThreeTensor::write(std::ofstream& os) const {
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

  std::ifstream& MatrixThreeTensor::read(std::ifstream& is) {
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

  void MatrixThreeTensor::rescale(double weight, std::shared_ptr<const ThreeTensor> densities) {
    // Re-weight by m_weight, and scale from energy to dose
    for (int zi = 0; zi < this->nz(); ++zi) {
      for (int yi = 0; yi < this->ny(); ++yi) {
        for (int xi = 0; xi < this->nx(); ++xi) {
          (*this)(xi, yi, zi) *= weight / (*densities)(xi, yi, zi);
        }
      }
    }
  }
};
