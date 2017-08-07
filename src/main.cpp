#include <chrono>
#include <iostream>
#include "roulette/controller.h"

int main(int argc, char *argv[]) {
  auto start = std::chrono::system_clock::now();

  if (argc > 1) {
    roulette::Controller controller(argv[1]);
    controller.run();
  }

  auto end = std::chrono::system_clock::now();
  auto diff = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
  std::cout << diff.count()/1000000000.0 << " seconds" << std::endl;

  return 0;
}
