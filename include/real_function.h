#pragma once

#include <limits>
#include <functional>

namespace roulette {
  class RealFunction {
    private:
      double m_xmin;
      double m_xmax;
      std::function<double(double)> m_function;

    public:
      RealFunction(std::function<double(double)> function, double xmin = -std::numeric_limits<double>::infinity(), double xmax = std::numeric_limits<double>::infinity());

      double xmin() const;
      double xmax() const;
      double operator()(double x) const;

      static RealFunction constant_function(double y);
  };
};
