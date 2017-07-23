#include <cmath>

#include "roulette/distributions/compton_scattering.h"

#include "roulette/electron.h"

namespace roulette {
  namespace distributions {
    ComptonScattering::ComptonScattering() {}

    double ComptonScattering::operator()(RandomGenerator& generator) {
      double x, sin2theta;

      while (true) {
        // Sample from f1
        if (generator.uniform() < alpha1 / (alpha1+alpha2)) {
          x = std::sqrt(2*generator.uniform()/A1 + Emin*Emin);
        }
        // Sample from f2
        else {
          x = Emin * std::exp(generator.uniform() / A2);
        }

        sin2theta = (1 - 1/x + 1/m_photon_E_0);
        sin2theta = 1 - sin2theta*sin2theta;

        if (generator.uniform() < 1 - sin2theta/(x/m_photon_E_0 + m_photon_E_0/x)) {
          break;
        }
      }

      this->set_final_photon_energy(x);

      m_phi = 2*M_PI*generator.uniform();

      return m_photon_E_1;
    }

    void ComptonScattering::set_initial_photon(const Photon& photon) {
      this->set_initial_photon_energy_in_units_of_electron_mass(photon.energy() / ELECTRON_REST_ENERGY_IN_EV);
    }

    void ComptonScattering::set_initial_photon_energy_in_units_of_electron_mass(double e) {
      m_photon_E_0 = e;

      Emin = m_photon_E_0 / (1 + 2*m_photon_E_0);
      Emax = m_photon_E_0;

      // 2/E^2 * (1 + 4E + 4E^2) / (4E + 4E^2)
      A1 = 4*m_photon_E_0*(1+m_photon_E_0);
      A1 = 2.0 / (m_photon_E_0*m_photon_E_0) * (1 + A1) / A1;

      A2 = 1 / std::log(1+2*m_photon_E_0);

      alpha1 = 1.0 / (A1 * m_photon_E_0);
      alpha2 = m_photon_E_0 / A2;
    }

    double ComptonScattering::initial_photon_energy() const { return m_photon_E_0; }

    double ComptonScattering::final_photon_energy() const { return m_photon_E_1; }
    double ComptonScattering::final_electron_energy() const { return m_electron_E_1; }
    double ComptonScattering::final_photon_theta() const { return m_photon_theta; }
    double ComptonScattering::final_electron_theta() const { return m_electron_theta; }
    double ComptonScattering::final_phi() const { return m_phi; }

    double ComptonScattering::area_between(double e0, double e1) const {
      return normalized_cdf(e1) - normalized_cdf(e0);
    }

    void ComptonScattering::fill_in_expected_histogram(Histogram& expected, int num_samples) const {
      expected.fill_in_from_cdf(
        std::bind(&ComptonScattering::area_between, this, std::placeholders::_1, std::placeholders::_2),
        num_samples
      );
    }

    FourMomentum ComptonScattering::initial_photon_momentum() const {
      double factor = ELECTRON_REST_ENERGY_IN_EV*m_photon_E_0;
      return FourMomentum(factor, factor, 0, 0);
    }

    FourMomentum ComptonScattering::initial_electron_momentum() const {
      return FourMomentum(ELECTRON_REST_ENERGY_IN_EV, 0, 0, 0);
    }

    FourMomentum ComptonScattering::final_photon_momentum() const {
      double factor = ELECTRON_REST_ENERGY_IN_EV*m_photon_E_1;
      return FourMomentum(
        factor,
        factor*std::cos(m_photon_theta),
        factor*std::sin(m_photon_theta),
        0
      );
    }

    FourMomentum ComptonScattering::final_electron_momentum() const {
      double momentum_magnitude = ELECTRON_MASS_IN_EV*std::sqrt(m_electron_E_1*m_electron_E_1 - 1);
      return FourMomentum(
        ELECTRON_REST_ENERGY_IN_EV*m_electron_E_1,
        momentum_magnitude * std::cos(m_electron_theta),
        momentum_magnitude * std::sin(m_electron_theta),
        0
      );
    }

    // PDF: A * (E' / E + E / E' - sin^2(theta))
    // where theta is given, by relativistic kinematics, from: cos(theta) = 1 - 1/E' + 1/E
    double ComptonScattering::cdf(double e) const {
      // Fix range
      e = std::max(e, Emin);
      e = std::min(e, Emax);

      return ((e*e - Emin*Emin)/2/m_photon_E_0) + std::log(e/Emin)*(m_photon_E_0 - 2.0/m_photon_E_0 - 2.0) +
        (e - Emin)*(1/m_photon_E_0/m_photon_E_0 + 2/m_photon_E_0) - 1.0/e + 1.0/m_photon_E_0;
    }

    double ComptonScattering::normalized_cdf(double e) const {
      return cdf(e) / cdf(Emax);
    }

    void ComptonScattering::set_final_photon_energy(double e) {
      m_photon_E_1 = e;
      m_photon_theta = std::acos(1 - 1.0/m_photon_E_1 + 1.0/m_photon_E_0);

      // Conservation of energy: E + 1 = E' + E_e
      m_electron_E_1 = m_photon_E_0 + 1 - m_photon_E_1;

      // Conservation of momentum:
      // x: E' cos(theta_gamma) + sqrt(E_e^2 - 1) cos(theta_electron) = E
      // y: E' sin(theta_gamma) + sqrt(E_e^2 - 1) sin(theta_electron) = 0
      // Therefore electron 4-momentum p_e / mc = (E_e, px/mc, py/mc, 0)
      // with px/mc = E - E' cos(theta_gamma)
      //      py/mc = -E' sin(theta_gamma)
      m_electron_theta = std::atan2(-m_photon_E_1 * std::sin(m_photon_theta), m_photon_E_0 - m_photon_E_1 * std::cos(m_photon_theta));
    }
  };
};
