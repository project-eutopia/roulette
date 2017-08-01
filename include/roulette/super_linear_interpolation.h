#pragma once

#include <vector>
#include <functional>

// Uses pairs of (x,y) evenly spaced in x to approximate the given function.
// Better fitting that just choosing the y = f(x), because we actually minimize
// the error function int_x0^xN (f(x) - g(x,y_i))^2 where g(x,y_i) is the linearly
// fit function given variables y_i for i = 1, ... N-1, and y0 = f(x0) and yN = f(xN)
namespace roulette {
  template <typename F>
  class SuperLinearInterpolation {
    private:
      F m_f;
      std::vector<double> m_xs;
      std::vector<double> m_ys;
      double m_delta_x;

    public:
      SuperLinearInterpolation(F f, std::function<double(double)> integral_f, std::function<double(double)> integral_xf, double x0, double xN, int N);

      const std::vector<double>& xs() const;
      const std::vector<double>& ys() const;

      double operator()(double x) const;
  };
};

#include "roulette/super_linear_interpolation_impl.h"
