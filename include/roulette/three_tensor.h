#pragma once

#include <boost/multi_array.hpp>

namespace roulette {
  class ThreeTensor {
    private:
      // Shape (nz, ny, nx)
      boost::multi_array<double,3> m_tensor;

    public:
      ThreeTensor(const boost::multi_array<double,3>& tensor);
      ThreeTensor(int nz, int ny, int nx, double fill_value = 0);

      int nz() const;
      int ny() const;
      int nx() const;

      double operator()(int zi, int yi, int xi) const;
      double& operator()(int zi, int yi, int xi);
  };
};
