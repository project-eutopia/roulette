#include "roulette/photon.h"
#include "roulette/lorentz_transform.h"

namespace roulette {
  double Photon::mass() const { return Photon::MASS; }
  int Photon::charge() const { return Photon::CHARGE; }

  Electron Photon::compton_scatter(double photon_energy, double electron_energy, double photon_theta, double electron_theta, double phi) {
    LorentzTransform rotate_to_initial;
    int xsign;

    // To avoid degenerate rotations, consider rotation matrix from (1,0,0) to photon direction,
    // or (-1,0,0) to photon direction
    if (m_momentum.px() >= 0) {
      xsign = 1;
      rotate_to_initial = LorentzTransform::rotationUtoV(ThreeVector(1,0,0), this->momentum().direction_unit_vector());
    }
    else {
      xsign = -1;
      rotate_to_initial = LorentzTransform::rotationUtoV(ThreeVector(-1,0,0), this->momentum().direction_unit_vector());
    }

    double sin_p = std::sin(photon_theta);
    double cos_p = std::cos(photon_theta);
    double sin_e = std::sin(electron_theta);
    double cos_e = std::cos(electron_theta);
    double sin_phi = std::sin(phi);
    double cos_phi = std::cos(phi);

    m_momentum = rotate_to_initial * FourMomentum(photon_energy, xsign * cos_p * photon_energy, sin_p * cos_phi * photon_energy, sin_p * sin_phi * photon_energy);
    m_energy = photon_energy;

    double e_momentum = std::sqrt(electron_energy*electron_energy - Electron::MASS*Electron::MASS);
    return Electron(
      rotate_to_initial * FourMomentum(electron_energy, xsign * cos_e * e_momentum, sin_e * cos_phi * e_momentum, sin_e * sin_phi * e_momentum),
      ThreeVector(this->position()(0), this->position()(1), this->position()(2)),
      m_weight
    );
  }

  void Photon::deposit_energy(SourceSimulation& source_simulation) {
    // TODO
    /* // Roulette to eliminate low energy photons (below 100 keV) */
    /* if (this->energy() < 10000) { */
    /*   // 1 in 3 chance of skipping */
    /*   if (source_simulation.generator().uniform() > 2.0/3.0) { */
    /*     return; */
    /*   } */
    /*   this->weight() *= 3.0/2.0; */
    /* } */

    /* double energy = this->energy(); */

    /* Particle::InteractionType interaction = Particle::InteractionType::NONE; */

    /* ThreeVector final_position = source_simulation.phantom().ray_trace_voxels( */
    /*   this->position(), this->momentum().three_momentum(), */
    /*   Phantom::voxel_iterator( */
    /*     [&](const Phantom& cur_phantom, double distance, int xi, int yi, int zi) -> double { */
    /*       // Check for scatter */
    /*       if (source_simulation.generator().uniform() < m_interaction_function(cur_phantom(xi, yi, zi) * cur_phantom.compound(xi, yi, zi).photon_scattering_cross_section(energy) * distance)) { */
    /*         interaction = Particle::InteractionType::PHOTON_SCATTER; */
    /*         return distance / 2.0; */
    /*       } */
    /*       // Check for photoelectric effect */
    /*       else if (source_simulation.generator().uniform() < m_interaction_function(cur_phantom(xi, yi, zi) * cur_phantom.compound(xi, yi, zi).photon_absorption_cross_section(energy) * distance)) { */
    /*         interaction = Particle::InteractionType::PHOTON_PHOTOELECTRIC; */
    /*         return distance / 2.0; */
    /*       } */

    /*       return distance; */
    /*     } */
    /*   ) */
    /* ); */

    /* if (interaction == Particle::InteractionType::PHOTON_SCATTER) { */
    /*   this->position() = final_position; */

    /*   m_compton_scattering.set_initial_photon(*this); */
    /*   m_compton_scattering(source_simulation.generator()); */
    /*   Electron electron = this->compton_scatter(m_compton_scattering.final_photon_energy(), m_compton_scattering.final_electron_energy(), m_compton_scattering.final_photon_theta(), m_compton_scattering.final_electron_theta(), m_compton_scattering.final_phi()); */

    /*   electron.deposit_energy(source_simulation.generator(), phantom, dose); */
    /*   this->deposit_energy(source_simulation.generator(), phantom, dose); */
    /* } */
    /* else if (interaction == Particle::InteractionType::PHOTON_PHOTOELECTRIC) { */
    /*   Electron electron(this->energy() + Electron::MASS, Electron::MASS, m_spherical(*m_generator), final_position, this->weight()); */
    /*   electron.deposit_energy(source_simulation.generator(), phantom, dose); */
    /* } */
  }
};

std::ostream &operator<<(std::ostream &os, const roulette::Photon& p) {
  os << "Photon(" << p.momentum() << ", " << p.position() << ", weight=" << p.weight() << ")";
  return os;
}
