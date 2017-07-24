#pragma once

#include <memory>

#include "roulette/real_function.h"
#include "roulette/non_uniform_linear_interpolation.h"

namespace roulette {
  class Material {
    private:
      double m_density;
      NonUniformLinearInterpolation m_photon_mass_attenuation;

    public:
      Material(double density, const NonUniformLinearInterpolation& photon_mass_attenuation);

      double density() const;
      double photon_mass_attenuation(double energy) const;
  };
};
