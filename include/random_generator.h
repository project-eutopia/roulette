#pragma once

#include <random>

namespace roulette {
  class RandomGenerator {
    private:
      unsigned int m_seed;

      // Random number generator
      std::mt19937 m_generator;

      // Distributions
      std::uniform_real_distribution<double> m_uniform;

    public:
      RandomGenerator();
      RandomGenerator(unsigned int seed);

      static unsigned int random_seed();

      std::mt19937& engine();
      double uniform();
  };
};
