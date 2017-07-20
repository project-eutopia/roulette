#include <iostream>
#include "random_generator.h"
#include "distributions/built_in.h"

int main() {
  std::cout << "hello" << std::endl;
  roulette::RandomGenerator r;
  std::cout << "rand: " << r.uniform() << std::endl;
  std::cout << "rand: " << r.uniform() << std::endl;
  std::cout << "rand: " << r.uniform() << std::endl;

  roulette::distributions::BuiltIn<std::uniform_real_distribution<double>> uniform(std::uniform_real_distribution<double>(5, 10));
  std::cout << "uniform: " << uniform(r) << std::endl;
  std::cout << "uniform: " << uniform(r) << std::endl;
  std::cout << "uniform: " << uniform(r) << std::endl;

  roulette::distributions::BuiltIn<std::normal_distribution<double>> norm(std::normal_distribution<double>(0, 1));
  std::cout << "norm: " << norm(r) << std::endl;
  std::cout << "norm: " << norm(r) << std::endl;
  std::cout << "norm: " << norm(r) << std::endl;

  return 0;
}
