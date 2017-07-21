#pragma once

#include "roulette/random_generator.h"
#include "roulette/distribution.h"
#include "roulette/histogram.h"

namespace roulette {
  namespace distributions {
    // Energies are in units of m_e c^2 (electron rest energy)
    //
    // Implementation described in section 2.8 of "Monte Carlo Methods for Radiation Transport"
    // by Oleg N. Vassiliev
    class ComptonScattering : public Distribution {
      private:
        double m_photon_E_0;
        double m_photon_E_1;
        double m_electron_E_1;
        double m_photon_theta;
        double m_electron_theta;

        double Emin, Emax, A1, A2, alpha1, alpha2;

      public:
        ComptonScattering();
        // Returns new E' for given E, and sets other associated variables
        double operator()(RandomGenerator& generator);

        void set_initial_photon_energy(double e);

        double initial_photon_energy() const;

        double final_photon_energy() const;
        double final_electron_energy() const;
        double final_photon_theta() const;
        double final_electron_theta() const;

        double area_between(double e0, double e1) const;
        void fill_in_expected_histogram(Histogram& expected, int num_samples = 1) const;

      private:
        double cdf(double e) const;
        double normalized_cdf(double e) const;
        void set_final_photon_energy(double e);
    };
  };
};
