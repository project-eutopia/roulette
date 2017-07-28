#pragma once

#include <map>

#include "roulette/non_uniform_linear_interpolation.h"
#include "roulette/periodic_table.h"

#include "rapidjson/document.h"

namespace roulette {
  class Compound {
    private:
      std::string m_name;
      double m_z_over_a_ratio;
      double m_excitation_energy; // eV
      double m_density; // g/cm^3

      std::map <int,double> m_composition;

      NonUniformLinearInterpolation m_photon_scattering_cross_sections;
      NonUniformLinearInterpolation m_photon_absorption_cross_sections;
      NonUniformLinearInterpolation m_photon_pair_production_cross_sections;

      NonUniformLinearInterpolation m_electron_stopping_powers;

    public:
      Compound(const rapidjson::Value& data, const PeriodicTable& periodic_table);

      const std::string& name() const;
      double z_over_a_ratio() const;
      double excitation_energy() const;
      double density() const;

      const std::map<int,double>& composition() const;

      // cm^2 / g
      double photon_scattering_cross_section(double energy) const;
      double photon_absorption_cross_section(double energy) const;
      double photon_pair_production_cross_section(double energy) const;
      // eV cm^2 / g
      double electron_stopping_power(double energy) const;
  };
};
