#include "roulette/distributions/spherical.h"

#include <cmath>

namespace roulette {
  namespace distributions {
    Spherical::Spherical() {
    }

    ThreeVector Spherical::operator()(RandomGenerator& generator) const {
      double phi = 2*M_PI*generator.uniform();
      double z = 2*generator.uniform() - 1;
      double r = std::sqrt(1 - z*z);

      return ThreeVector(
        r*std::cos(phi),
        r*std::sin(phi),
        z
      );
    }
  };
};
