#include "roulette/math.h"

namespace roulette {
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
};
