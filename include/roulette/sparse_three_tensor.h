#pragma once

#include "roulette/three_tensor.h"

#include <map>
#include <string>

namespace roulette {
  class SparseThreeTensor : public ThreeTensor {
    private:
      int m_nx;
      int m_ny;
      int m_nz;
      double m_default;
      std::map<int,double> m_data;

    public:
      SparseThreeTensor();
      SparseThreeTensor(int nx, int ny, int nz, double fill_value = 0);
      SparseThreeTensor(std::string filename);

      int nx() const;
      int ny() const;
      int nz() const;

      double operator()(int xi, int yi, int zi) const;
      double& operator()(int xi, int yi, int zi);

      std::ofstream& write(std::ofstream& os) const;
      std::ifstream& read(std::ifstream& is);

      void rescale(double weight, std::shared_ptr<const ThreeTensor> densities);
  };
};
