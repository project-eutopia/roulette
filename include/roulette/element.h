#pragma once

#include "roulette/non_uniform_linear_interpolation.h"

#include "rapidjson/document.h"

namespace roulette {
  class Element {
    private:
      std::string m_name;
      std::string m_symbol;
      int m_atomic_number;
      double m_z_over_a_ratio;
      double m_excitation_energy; // eV
      double m_density; // g/cm^3

      NonUniformLinearInterpolation m_photon_scattering_cross_sections;
      NonUniformLinearInterpolation m_photon_absorption_cross_sections;
      NonUniformLinearInterpolation m_photon_pair_production_cross_sections;

      NonUniformLinearInterpolation m_electron_stopping_powers;
      NonUniformLinearInterpolation m_electron_csda_ranges;

    public:
      Element(const rapidjson::Value& data);

      const std::string& name() const;
      const std::string& symbol() const;
      int atomic_number() const;
      double z_over_a_ratio() const;
      double excitation_energy() const;
      double density() const;

      // cm^2 / g
      const NonUniformLinearInterpolation& photon_scattering_cross_sections() const;
      double photon_scattering_cross_section(double energy) const;

      const NonUniformLinearInterpolation& photon_absorption_cross_sections() const;
      double photon_absorption_cross_section(double energy) const;

      const NonUniformLinearInterpolation& photon_pair_production_cross_sections() const;
      double photon_pair_production_cross_section(double energy) const;

      // eV cm^2 / g
      const NonUniformLinearInterpolation& electron_stopping_powers() const;
      double electron_stopping_power(double energy) const;

      // g / cm^2
      const NonUniformLinearInterpolation& electron_csda_ranges() const;
      double electron_csda_range(double energy) const;
  };
};
