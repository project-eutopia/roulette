#pragma once

#include <vector>
#include <string>

namespace roulette {
  class NonUniformLinearInterpolation {
    private:
      std::vector<double> m_x;
      std::vector<double> m_y;

    public:
      NonUniformLinearInterpolation();
      NonUniformLinearInterpolation(const std::vector<double>& x, const std::vector<double>& y);
      NonUniformLinearInterpolation(std::vector<double>&& x, std::vector<double>&& y);
      NonUniformLinearInterpolation(const std::string& filename);

      // Must add in increasing x order!
      void add_point(double x, double y);

      const std::vector<double>& xs() const;
      const std::vector<double>& ys() const;

      double operator()(double x) const;
  };
};
