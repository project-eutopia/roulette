#pragma once

#include <vector>
#include <string>

namespace roulette {
  class ThreeTensor {
    private:
      int m_nx;
      int m_ny;
      int m_nz;
      std::vector<double> m_data;

    public:
      ThreeTensor(int nx, int ny, int nz, double fill_value = 0);

      int nx() const;
      int ny() const;
      int nz() const;

      double operator()(int xi, int yi, int zi) const;
      double& operator()(int xi, int yi, int zi);

      void write_to_file(std::string filename) const;
  };
};
