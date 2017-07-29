#pragma once

#include <vector>
#include <string>

namespace roulette {
  class TwoTensor {
    private:
      int m_nx;
      int m_ny;
      std::vector<double> m_data;

    public:
      TwoTensor(int nx, int ny, double fill_value = 0);

      int nx() const;
      int ny() const;

      double operator()(int xi, int yi) const;
      double& operator()(int xi, int yi);

      void write_to_file(std::string filename) const;
  };
};
