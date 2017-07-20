#include "real_function.h"

namespace roulette {
  RealFunction::RealFunction(std::function<double(double)> function, double xmin, double xmax) :
    m_xmin(xmin),
    m_xmax(xmax),
    m_function(function)
  {
  };

  double RealFunction::xmin() const { return m_xmin; }
  double RealFunction::xmax() const { return m_xmax; }

  double RealFunction::operator()(double x) const {
    return m_function(x);
  };
};
