#include "roulette/compound.h"

namespace roulette {
  Compound::Compound() {}

  Compound::Compound(const rapidjson::Value& data, const PeriodicTable& periodic_table) :
    m_name(data["name"].GetString()),
    m_z_over_a_ratio(data["z_over_a"].GetDouble()),
    m_excitation_energy(data["excitation_energy"].GetDouble()),
    m_density(data["density"].GetDouble())
  {
    for (const auto& m : data["composition"].GetObject()) {
      m_composition.insert(
        std::make_pair(
          std::stoi(m.name.GetString()),
          m.value.GetDouble()
        )
      );
    }

    std::vector<double> weights;
    std::vector<const NonUniformLinearInterpolation*> photon_scattering_cross_sections;
    std::vector<const NonUniformLinearInterpolation*> photon_absorption_cross_sections;
    std::vector<const NonUniformLinearInterpolation*> photon_pair_production_cross_sections;
    std::vector<const NonUniformLinearInterpolation*> electron_stopping_powers;
    std::vector<const NonUniformLinearInterpolation*> electron_csda_ranges;

    for (auto& kv : m_composition) {
      const Element& element = periodic_table.element(kv.first);

      weights.push_back(kv.second);
      photon_scattering_cross_sections.push_back(&element.photon_scattering_cross_sections());
      photon_absorption_cross_sections.push_back(&element.photon_absorption_cross_sections());
      photon_pair_production_cross_sections.push_back(&element.photon_pair_production_cross_sections());
      electron_stopping_powers.push_back(&element.electron_stopping_powers());
      electron_csda_ranges.push_back(&element.electron_csda_ranges());
    }

    m_photon_scattering_cross_sections = NonUniformLinearInterpolation::linear_combination(
      photon_scattering_cross_sections, weights
    );
    m_photon_absorption_cross_sections = NonUniformLinearInterpolation::linear_combination(
      photon_absorption_cross_sections, weights
    );
    m_photon_pair_production_cross_sections = NonUniformLinearInterpolation::linear_combination(
      photon_pair_production_cross_sections, weights
    );
    m_electron_stopping_powers = NonUniformLinearInterpolation::linear_combination(
      electron_stopping_powers, weights
    );
    m_electron_csda_ranges = NonUniformLinearInterpolation::linear_combination(
      electron_csda_ranges, weights
    );
  }

  Compound::Compound(const Element& element) :
    m_name(element.name()),
    m_z_over_a_ratio(element.z_over_a_ratio()),
    m_excitation_energy(element.excitation_energy()),
    m_density(element.density()),
    m_photon_scattering_cross_sections(
      element.photon_scattering_cross_sections().xs(),
      element.photon_scattering_cross_sections().ys()
    ),
    m_photon_absorption_cross_sections(
      element.photon_absorption_cross_sections().xs(),
      element.photon_absorption_cross_sections().ys()
    ),
    m_photon_pair_production_cross_sections(
      element.photon_pair_production_cross_sections().xs(),
      element.photon_pair_production_cross_sections().ys()
    ),
    m_electron_stopping_powers(
      element.electron_stopping_powers().xs(),
      element.electron_stopping_powers().ys()
    ),
    m_electron_csda_ranges(
      element.electron_csda_ranges().xs(),
      element.electron_csda_ranges().ys()
    )
  {
    m_composition[element.atomic_number()] = 1.0;
  }

  const std::string& Compound::name() const { return m_name; }
  double Compound::z_over_a_ratio() const { return m_z_over_a_ratio; }
  double Compound::excitation_energy() const { return m_excitation_energy; }
  double Compound::density() const { return m_density; }

  const std::map<int,double>& Compound::composition() const { return m_composition; }

  // cm^2 / g
  double Compound::photon_scattering_cross_section(double energy) const {
    return m_photon_scattering_cross_sections(energy);
  }
  double Compound::photon_absorption_cross_section(double energy) const {
    return m_photon_absorption_cross_sections(energy);
  }
  double Compound::photon_pair_production_cross_section(double energy) const {
    return m_photon_pair_production_cross_sections(energy);
  }
  double Compound::photon_total_cross_section(double energy) const {
    // FIXME: Add pair production when implemented
    return m_photon_scattering_cross_sections(energy) + m_photon_absorption_cross_sections(energy);
  }
  // eV cm^2 / g
  double Compound::electron_stopping_power(double energy) const {
    return m_electron_stopping_powers(energy);
  }
  // g / cm^2
  double Compound::electron_csda_range(double energy) const {
    return m_electron_csda_ranges(energy);
  }
};
