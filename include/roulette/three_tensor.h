#pragma once

#include <vector>
#include <string>

namespace roulette {
  class ThreeTensor {
    public:
      virtual int nx() const = 0;
      virtual int ny() const = 0;
      virtual int nz() const = 0;

      virtual double operator()(int xi, int yi, int zi) const = 0;
      virtual double& operator()(int xi, int yi, int zi) = 0;

      virtual std::ofstream& write(std::ofstream& os) const = 0;
      virtual std::ifstream& read(std::ifstream& is) = 0;
  };
};
