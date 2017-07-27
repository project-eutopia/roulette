#pragma once

#include <memory>

#include "roulette/real_function.h"
#include "roulette/non_uniform_linear_interpolation.h"

namespace roulette {
  class Material {
    private:
      double m_density;
      NonUniformLinearInterpolation m_photon_mass_attenuation;
      NonUniformLinearInterpolation m_electron_stopping_power;

    public:
      Material(std::string filename);
      Material(double density, const NonUniformLinearInterpolation& photon_mass_attenuation, const NonUniformLinearInterpolation& electron_stopping_power);

      // Units: g/cm^3
      double density() const;
      // Units cm^2/g
      double photon_mass_attenuation(double energy) const;
      // Units cm^2/g
      double electron_stopping_power(double kinetic_energy) const;
  };
};
