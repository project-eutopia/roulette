#include "roulette/electron.h"

namespace roulette {
  double Electron::mass() const { return Electron::MASS; }
  int Electron::charge() const { return Electron::CHARGE; }

  void Electron::deposit_energy(SourceDose& source_dose) {
    double kinetic_energy = this->kinetic_energy();

    const Phantom& phantom = source_dose.phantom();
    phantom.ray_trace_voxels(
      this->position(), this->momentum().three_momentum(),
      IVoxelGrid::voxel_iterator(
        [&,this](double distance, size_t xi, size_t yi, size_t zi) -> double {
          double csda_range_cm = phantom.compound(xi, yi, zi).electron_csda_range(kinetic_energy) / phantom(xi, yi, zi);

          double energy_drop = (distance <= csda_range_cm) ? kinetic_energy*(distance / csda_range_cm) : kinetic_energy;
          source_dose.dose()->increment(xi, yi, zi, this->weight()*energy_drop);
          kinetic_energy -= energy_drop;

          // Roulette to decide if we keep going
          if (kinetic_energy < 10000) {
            // 1 in 3 chance of skipping
            if (source_dose.generator().uniform() > 2.0/3.0) {
              return 0;
            }
            this->weight() *= 3.0/2.0;
          }

          return std::min(csda_range_cm, distance);
        }
      )
    );
  }

  std::shared_ptr<Event> Electron::simulate(RandomGenerator& generator, const Phantom& phantom) {
    double kinetic_energy = this->kinetic_energy();
    ThreeVector initial_position = this->position();
    FourMomentum initial_momentum = this->momentum();

    ThreeVector final_position = phantom.ray_trace_voxels(
      this->position(), this->momentum().three_momentum(),
      IVoxelGrid::voxel_iterator(
        [&,this](double distance, size_t xi, size_t yi, size_t zi) -> double {
          double csda_range_cm = phantom.compound(xi, yi, zi).electron_csda_range(kinetic_energy) / phantom(xi, yi, zi);

          double energy_drop = (distance <= csda_range_cm) ? kinetic_energy*(distance / csda_range_cm) : kinetic_energy;
          kinetic_energy -= energy_drop;

          // Roulette to decide if we keep going
          if (kinetic_energy < 10000) {
            // 1 in 3 chance of skipping
            if (generator.uniform() > 2.0/3.0) {
              return 0;
            }
            this->weight() *= 3.0/2.0;
          }

          return std::min(csda_range_cm, distance);
        }
      )
    );

    return std::make_shared<Event>(
      Event::EventType::ELECTRON_ABSORB,
      Event::ParticleType::ELECTRON,
      initial_momentum,
      initial_position,
      final_position
    );
  }
};

std::ostream &operator<<(std::ostream &os, const roulette::Electron& e) {
  os << "Electron(" << e.momentum() << ", " << e.position() << ", weight=" << e.weight() << ")";
  return os;
}
