#include "roulette/material.h"

#include <fstream>
#include <string>

#include "rapidjson/document.h"

namespace roulette {
  Material::Material(std::string filename) {
    std::ifstream json_file(filename);
    std::string json_content( (std::istreambuf_iterator<char>(json_file) ),
                              (std::istreambuf_iterator<char>()    ) );

    rapidjson::Document data;
    data.Parse(json_content.c_str());

    if (data.HasMember("density")) {
      assert(data.IsNumber());
      m_density = data["density"].GetDouble();
    }

    if (data.HasMember("photon_mass_attenuation")) {
      const auto& attenuation = data["photon_mass_attenuation"];

      assert(attenuation.IsObject());
      assert(attenuation.HasMember("data"));

      const auto& attenuation_data = attenuation["data"];

      for (auto it = attenuation_data.Begin(); it != attenuation_data.End(); ++it) {
        m_photon_mass_attenuation.add_point(
          it->GetArray()[0].GetDouble(),
          it->GetArray()[1].GetDouble()
        );
      }
    }

    if (data.HasMember("electron_stopping_power")) {
      const auto& stopping_power = data["electron_stopping_power"];

      assert(stopping_power.IsObject());
      assert(stopping_power.HasMember("data"));

      const auto& stopping_power_data = stopping_power["stopping_power"];

      for (auto it = stopping_power_data.Begin(); it != stopping_power_data.End(); ++it) {
        m_electron_stopping_power.add_point(
          it->GetArray()[0].GetDouble(),
          it->GetArray()[1].GetDouble()
        );
      }
    }
  }

  double Material::density() const { return m_density; }

  double Material::photon_mass_attenuation(double energy) const {
    // NOTE: Tables are stored in MeV
    return m_photon_mass_attenuation(energy / 1000000);
  }

  // Units eV cm^2/g
  double Material::electron_stopping_power(double kinetic_energy) const {
    return m_electron_stopping_power(kinetic_energy / 1000000);
  }
};
