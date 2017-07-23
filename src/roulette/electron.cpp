#include "roulette/electron.h"

namespace roulette {
  double Electron::mass() const { return Electron::MASS; }
  int Electron::charge() const { return Electron::CHARGE; }
};

std::ostream &operator<<(std::ostream &os, const roulette::Electron& e) {
  os << "Electron(" << e.momentum() << ", " << e.position() << ")";
  return os;
}
