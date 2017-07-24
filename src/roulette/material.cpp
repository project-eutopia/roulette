#include "roulette/material.h"

namespace roulette {
  Material::Material(double density, const NonUniformLinearInterpolation& photon_mass_attenuation) :
    m_density(density),
    m_photon_mass_attenuation(photon_mass_attenuation)
  {
  }

  double Material::density() const { return m_density; }

  double Material::photon_mass_attenuation(double energy) const {
    return m_photon_mass_attenuation(energy);
  }
};
