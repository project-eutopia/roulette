#include "roulette/photon.h"
#include "roulette/lorentz_transform.h"
#include "roulette/distributions/compton_scattering.h"

namespace roulette {
  const distributions::Exponential Photon::exponential = distributions::Exponential(1);
  const distributions::Spherical Photon::spherical = distributions::Spherical();

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

  void Photon::deposit_energy(SourceDose& source_dose) {
    // Roulette to eliminate low energy photons (below 10 keV)
    if (this->energy() < 10000) {
      // 1 in 3 chance of skipping
      if (source_dose.generator().uniform() > 2.0/3.0) {
        return;
      }
      this->weight() *= 3.0/2.0;
    }

    // Sample unitless depth travelled
    double depth = Photon::exponential(source_dose.generator());
    bool inside = source_dose.phantom().transport_photon_unitless_depth(*this, depth);
    if (!inside) return;

    std::tuple<int,int,int> xyz = source_dose.phantom().index_at(this->position());
    const Compound& compound = source_dose.phantom().compound(std::get<0>(xyz), std::get<1>(xyz), std::get<2>(xyz));

    // Pick interaction type
    double compton = compound.photon_scattering_cross_section(this->energy());
    double photoelectric = compound.photon_absorption_cross_section(this->energy());

    if ((compton + photoelectric) * source_dose.generator().uniform() < compton) {
      distributions::ComptonScattering compton_scattering;

      // Compton scatter
      compton_scattering.set_initial_photon(*this);
      compton_scattering(source_dose.generator());
      Electron electron = this->compton_scatter(compton_scattering.final_photon_energy(), compton_scattering.final_electron_energy(), compton_scattering.final_photon_theta(), compton_scattering.final_electron_theta(), compton_scattering.final_phi());

      electron.deposit_energy(source_dose);
      this->deposit_energy(source_dose);
    }
    else {
      // Photoelectric effect
      Electron electron(this->energy() + Electron::MASS, Electron::MASS, Photon::spherical(source_dose.generator()), this->position(), this->weight());
      electron.deposit_energy(source_dose);
    }
  }
};

std::ostream &operator<<(std::ostream &os, const roulette::Photon& p) {
  os << "Photon(" << p.momentum() << ", " << p.position() << ", weight=" << p.weight() << ")";
  return os;
}
