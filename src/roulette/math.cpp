#include "roulette/math.h"
#include "roulette/super_linear_interpolation.h"
#include <cmath>

namespace roulette {
  namespace math {
    int floori(double x) {
      return (int) x - (x < (int) x);
    }

    double floorf(double x) {
      return (double) ((int)x - (x < (int) x));
    }

    int ceili(double x) {
      return (int) x + (x > (int) x);
    }

    double ceilf(double x) {
      return (double) ((int)x + (x > (int) x));
    }

    bool isinteger(double x) {
      return floorf(x) == x;
    }

    double sin(double theta) {
      static SuperLinearInterpolation<std::function<double(double)>> super_sin(
        [](double x) { return std::sin(x); },
        [](double x) { return -std::cos(x); },
        [](double x) { return std::sin(x) - x*std::cos(x); },
        0,
        2*M_PI,
        90
      );

      while (theta < 0) theta += 2*M_PI;
      while (theta >= 2*M_PI) theta -= 2*M_PI;
      return super_sin(theta);
    }

    double cos(double theta) {
      static SuperLinearInterpolation<std::function<double(double)>> super_cos(
        [](double x) { return std::cos(x); },
        [](double x) { return std::sin(x); },
        [](double x) { return std::cos(x) + x*std::sin(x); },
        0,
        2*M_PI,
        90
      );

      while (theta < 0) theta += 2*M_PI;
      while (theta >= 2*M_PI) theta -= 2*M_PI;
      return super_cos(theta);
    }
  }
};
