#include "roulette/interaction_function.h"

#include <cmath>
#include "roulette/math.h"

namespace roulette {
  InteractionFunction::InteractionFunction() :
    m_max_x(1.0),
    m_num_intervals(10),
    m_delta_x(m_max_x / m_num_intervals)
  {
    m_xs.reserve(m_num_intervals+1);
    m_ys.reserve(m_num_intervals+1);

    // TODO: linearly approximate without underestimating each interval
    double x, y;
    for (int i = 0; i <= m_num_intervals; ++i) {
      x = m_delta_x * i;
      y = 1 - std::exp(-x);
      m_xs.push_back(x);
      m_ys.push_back(y);
    }

    m_first_slope = (m_ys[1] - m_ys[0]) / (m_xs[1] - m_xs[0]);
  }

  double InteractionFunction::operator()(double x) const {
    if (x <= m_delta_x) {
      return x * m_first_slope;
    }
    else if (x < m_max_x) {
      int i = floori(x / m_delta_x);
      return m_ys[i] + (x - m_xs[i]) * (m_ys[i+1] - m_ys[i]) / (m_xs[i+1] - m_xs[i]);
    }
    else {
      return 1 - std::exp(-x);
    }
  }
};
