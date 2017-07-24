#include "roulette/three_tensor.h"

namespace roulette {
  ThreeTensor::ThreeTensor(const boost::multi_array<double,3>& tensor) :
    m_tensor(tensor)
  {}

  int ThreeTensor::nz() const { return m_tensor.shape()[0]; }
  int ThreeTensor::ny() const { return m_tensor.shape()[1]; }
  int ThreeTensor::nx() const { return m_tensor.shape()[2]; }

  double ThreeTensor::operator()(int zi, int yi, int xi) const { return m_tensor[zi][yi][xi]; }
  double& ThreeTensor::operator()(int zi, int yi, int xi) { return m_tensor[zi][yi][xi]; }
};
