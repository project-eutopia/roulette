#include "roulette/three_tensor.h"

namespace roulette {
  ThreeTensor::ThreeTensor(const boost::multi_array<double,3>& tensor) :
    m_tensor(tensor)
  {
  }

  ThreeTensor::ThreeTensor(int nx, int ny, int nz, double fill_value) :
    m_tensor(boost::extents[nz][ny][nx])
  {
    for (int zi = 0; zi < nz; ++zi) {
      for (int yi = 0; yi < ny; ++yi) {
        for (int xi = 0; xi < nx; ++xi) {
          m_tensor[zi][yi][xi] = fill_value;
        }
      }
    }
  }

  int ThreeTensor::nz() const { return m_tensor.shape()[0]; }
  int ThreeTensor::ny() const { return m_tensor.shape()[1]; }
  int ThreeTensor::nx() const { return m_tensor.shape()[2]; }

  double ThreeTensor::operator()(int xi, int yi, int zi) const { return m_tensor[zi][yi][xi]; }
  double& ThreeTensor::operator()(int xi, int yi, int zi) { return m_tensor[zi][yi][xi]; }
};
