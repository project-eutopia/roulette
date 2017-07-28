#pragma once

#include <vector>

namespace roulette {
  // Function: 1 - exp(-x)
  // But uses linear interpolation for values below 1
  class InteractionFunction {
    private:
      double m_max_x;
      int m_num_intervals;
      double m_delta_x;
      double m_first_slope;

      std::vector<double> m_xs;
      std::vector<double> m_ys;

    public:
      InteractionFunction();

      double operator()(double x) const;
  };
};
