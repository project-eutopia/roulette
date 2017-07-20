#include "random_generator.h"
#include <iostream>

namespace roulette {
  RandomGenerator::RandomGenerator() : RandomGenerator(RandomGenerator::random_seed()) {
  };

  RandomGenerator::RandomGenerator(unsigned int seed) :
    m_seed{seed},
    m_generator{seed},
    m_uniform{}
  {
  };

  unsigned int RandomGenerator::random_seed() {
    return std::random_device()();
  };

  std::mt19937& RandomGenerator::engine() {
    return m_generator;
  };

  double RandomGenerator::uniform() {
    return m_uniform(m_generator);
  };
};
