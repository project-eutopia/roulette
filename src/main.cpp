#include <iostream>
#include "roulette/random_generator.h"
#include "roulette/distributions/built_in.h"
#include "roulette/real_function.h"
#include "roulette/distributions/rejection_sampling.h"

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

  roulette::RealFunction quadratic([](double x) { return x*x; });
  std::cout << "quad: " << quadratic(0) << std::endl;
  std::cout << "quad: " << quadratic(1) << std::endl;
  std::cout << "quad: " << quadratic(2) << std::endl;
  std::cout << "quad: " << quadratic(3) << std::endl;

  roulette::RealFunction ten = roulette::RealFunction::constant_function(10);
  std::cout << "ten: " << ten(0) << std::endl;
  std::cout << "ten: " << ten(5) << std::endl;


  roulette::distributions::RejectionSampling rej(
    roulette::RealFunction([](double x) { return 0.75*(1-x*x); }),
    std::make_shared<roulette::distributions::BuiltIn<std::uniform_real_distribution<double>>>(std::uniform_real_distribution<double>(-1, 1)),
    roulette::RealFunction::constant_function(0.75)
  );

  std::cout << "rej: " << rej(r) << std::endl;
  std::cout << "rej: " << rej(r) << std::endl;
  std::cout << "rej: " << rej(r) << std::endl;
  std::cout << "rej: " << rej(r) << std::endl;
  std::cout << "rej: " << rej(r) << std::endl;

  return 0;
}
