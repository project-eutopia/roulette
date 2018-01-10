#pragma once

#include "roulette/three_tensor.h"

#include <vector>
#include <string>
#include <mutex>

namespace roulette {
  class MatrixThreeTensor : public ThreeTensor {
    private:
      int m_nx;
      int m_ny;
      int m_nz;
      std::vector<double> m_data;
      std::mutex m_data_mutex;

    public:
      MatrixThreeTensor();
      MatrixThreeTensor(int nx, int ny, int nz, double fill_value = 0);
      MatrixThreeTensor(std::string filename);

      int nx() const;
      int ny() const;
      int nz() const;

      double operator()(int xi, int yi, int zi) const;
      void set(int xi, int yi, int zi, double value);
      void increment(int xi, int yi, int zi, double delta);

      std::ofstream& write(std::ofstream& os) const;
      std::ifstream& read(std::ifstream& is);

      void rescale(double weight, std::shared_ptr<const ThreeTensor> densities);

    private:
      double& operator()(int xi, int yi, int zi);
  };
};
