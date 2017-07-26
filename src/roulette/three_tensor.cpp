#include "roulette/three_tensor.h"

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
};
