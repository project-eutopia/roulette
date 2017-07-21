#include "roulette/distributions/compton_scattering.h"

#include <cmath>

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

      return m_photon_E_1;
    }

    void ComptonScattering::set_initial_photon_energy(double e) {
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

    double ComptonScattering::area_between(double e0, double e1) const {
      return normalized_cdf(e1) - normalized_cdf(e0);
    }

    void ComptonScattering::fill_in_expected_histogram(Histogram& expected, int num_samples) const {
      expected.fill_in_from_cdf(
        std::bind(&ComptonScattering::area_between, this, std::placeholders::_1, std::placeholders::_2),
        num_samples
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
