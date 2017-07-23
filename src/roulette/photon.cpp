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

    double e_momentum = std::sqrt(electron_energy*electron_energy - Electron::MASS*Electron::MASS);
    return Electron(
      rotate_to_initial * FourMomentum(electron_energy, xsign * cos_e * e_momentum, sin_e * cos_phi * e_momentum, sin_e * sin_phi * e_momentum),
      ThreeVector(this->position()(0), this->position()(1), this->position()(2))
    );
  }
};

std::ostream &operator<<(std::ostream &os, const roulette::Photon& p) {
  os << "Photon(" << p.momentum() << ", " << p.position() << ")";
  return os;
}
