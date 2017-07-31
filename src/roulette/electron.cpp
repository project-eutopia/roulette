#include "roulette/electron.h"

namespace roulette {
  double Electron::mass() const { return Electron::MASS; }
  int Electron::charge() const { return Electron::CHARGE; }

  void Electron::deposit_energy(SourceSimulation& source_simulation) {
    double kinetic_energy = this->kinetic_energy();

    ThreeVector final_position = source_simulation.phantom().ray_trace_voxels(
      this->position(), this->momentum().three_momentum(),
      Phantom::voxel_iterator(
        [&,this](const Phantom& cur_phantom, double distance, int xi, int yi, int zi) -> double {
          double delta_energy = cur_phantom(xi, yi, zi) * cur_phantom.compound(xi, yi, zi).electron_stopping_power(kinetic_energy) * distance;

          double energy_drop = (delta_energy < kinetic_energy) ? delta_energy : kinetic_energy;

          source_simulation.dose()(xi, yi, zi) += this->weight() * energy_drop;
          kinetic_energy -= energy_drop;

          // Roulette to decide if we keep going
          if (kinetic_energy < 10000) {
            // 1 in 3 chance of skipping
            if (source_simulation.generator().uniform() > 2.0/3.0) {
              return 0;
            }
            this->weight() *= 3.0/2.0;
          }

          if (energy_drop < kinetic_energy) {
            return distance;
          }

          return energy_drop / cur_phantom(xi, yi, zi) / cur_phantom.compound(xi, yi, zi).electron_stopping_power(kinetic_energy);
        }
      )
    );
  }
};

std::ostream &operator<<(std::ostream &os, const roulette::Electron& e) {
  os << "Electron(" << e.momentum() << ", " << e.position() << ", weight=" << e.weight() << ")";
  return os;
}
