#pragma once

#include "roulette/random_generator.h"

namespace roulette {
  class Distribution {
    public:
      virtual double operator()(RandomGenerator& generator) = 0;
  };
};
