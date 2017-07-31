#include "roulette/controller.h"

int main(int argc, char *argv[]) {
  if (argc > 1) {
    roulette::Controller controller(argv[1]);
    controller.run();
  }

  return 0;
}
