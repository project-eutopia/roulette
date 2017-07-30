#include "roulette/electron.h"

namespace roulette {
  double Electron::mass() const { return Electron::MASS; }
  int Electron::charge() const { return Electron::CHARGE; }

  void Electron::deposit_energy(SourceSimulation& source_simulation) {
    //TODO
  }
};

std::ostream &operator<<(std::ostream &os, const roulette::Electron& e) {
  os << "Electron(" << e.momentum() << ", " << e.position() << ", weight=" << e.weight() << ")";
  return os;
}
