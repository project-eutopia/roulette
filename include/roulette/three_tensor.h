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
      ThreeTensor();
      ThreeTensor(int nx, int ny, int nz, double fill_value = 0);
      ThreeTensor(std::string filename);

      int nx() const;
      int ny() const;
      int nz() const;

      double operator()(int xi, int yi, int zi) const;
      double& operator()(int xi, int yi, int zi);

      std::ofstream& write(std::ofstream& os) const;
      std::ifstream& read(std::ifstream& is);
  };
};
