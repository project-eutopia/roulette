#include "roulette/element.h"

namespace roulette {
  Element::Element(const rapidjson::Value& data) :
    m_name(data["name"].GetString()),
    m_symbol(data["symbol"].GetString()),
    m_atomic_number(data["atomic_number"].GetInt()),
    m_z_over_a_ratio(data["z_over_a"].GetDouble()),
    m_excitation_energy(data["excitation_energy"].GetDouble()),
    m_density(data["density"].GetDouble())
  {
    const rapidjson::Value& photon_cross_sections = data["xray_cross_sections"];

    m_photon_scattering_cross_sections.reserve(photon_cross_sections.Size());
    m_photon_absorption_cross_sections.reserve(photon_cross_sections.Size());
    m_photon_pair_production_cross_sections.reserve(photon_cross_sections.Size());

    double energy;

    for (auto it = photon_cross_sections.Begin(); it != photon_cross_sections.End(); ++it) {
      // Convert from MeV to eV
      energy = (*it)["energy"].GetDouble() * 1000000.0;

      m_photon_scattering_cross_sections.add_point(
          energy,
          (*it)["coherent_scattering"].GetDouble() + (*it)["incoherent_scattering"].GetDouble()
      );

      m_photon_absorption_cross_sections.add_point(
          energy,
          (*it)["photoelectric_absorption"].GetDouble()
      );

      m_photon_pair_production_cross_sections.add_point(
          energy,
          (*it)["pair_production_nuclear_field"].GetDouble() + (*it)["pair_production_electric_field"].GetDouble()
      );
    }

    const rapidjson::Value& electron_stopping_powers = data["electron_stopping_powers"];
    m_electron_stopping_powers.reserve(electron_stopping_powers.Size());
    m_electron_csda_ranges.reserve(electron_stopping_powers.Size());

    for (auto it = electron_stopping_powers.Begin(); it != electron_stopping_powers.End(); ++it) {
      // Covert from MeV to eV
      energy = (*it)["energy"].GetDouble() * 1000000.0;

      m_electron_stopping_powers.add_point(
          energy,
          // Convert from MeV cm^2 / g to eV cm^2 / g
          (*it)["total_stopping_power"].GetDouble() * 1000000.0
      );

      m_electron_csda_ranges.add_point(
          energy,
          (*it)["csda_range"].GetDouble()
      );
    }
  }

  const std::string& Element::name() const { return m_name; }
  const std::string& Element::symbol() const { return m_symbol; }
  int Element::atomic_number() const { return m_atomic_number; }
  double Element::z_over_a_ratio() const { return m_z_over_a_ratio; }
  double Element::excitation_energy() const { return m_excitation_energy; }
  double Element::density() const { return m_density; }

  const NonUniformLinearInterpolation& Element::photon_scattering_cross_sections() const { return m_photon_scattering_cross_sections; }
  double Element::photon_scattering_cross_section(double energy) const {
    return m_photon_scattering_cross_sections(energy);
  }

  const NonUniformLinearInterpolation& Element::photon_absorption_cross_sections() const { return m_photon_absorption_cross_sections; }
  double Element::photon_absorption_cross_section(double energy) const {
    return m_photon_absorption_cross_sections(energy);
  }

  const NonUniformLinearInterpolation& Element::photon_pair_production_cross_sections() const { return m_photon_pair_production_cross_sections; }
  double Element::photon_pair_production_cross_section(double energy) const {
    return m_photon_pair_production_cross_sections(energy);
  }

  const NonUniformLinearInterpolation& Element::electron_stopping_powers() const { return m_electron_stopping_powers; }
  double Element::electron_stopping_power(double energy) const {
    return m_electron_stopping_powers(energy);
  }

  const NonUniformLinearInterpolation& Element::electron_csda_ranges() const { return m_electron_csda_ranges; }
  double Element::electron_csda_range(double energy) const {
    return m_electron_csda_ranges(energy);
  }
};
