#pragma once

namespace roulette {
  class Distribution {
    public:
      virtual double operator()(RandomGenerator& generator) = 0;
  };
};
