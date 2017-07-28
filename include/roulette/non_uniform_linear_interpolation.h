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

      void reserve(int n);

      // Must add in increasing x order!
      void add_point(double x, double y);

      const std::vector<double>& xs() const;
      const std::vector<double>& ys() const;

      double operator()(double x) const;

      // Uses first interpolation x values as arbitrary base of x's to evaluate at
      static NonUniformLinearInterpolation linear_combination(const std::vector<const NonUniformLinearInterpolation*>& interpolations, const std::vector<double>& weights);
  };
};
