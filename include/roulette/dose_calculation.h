#pragma once

#include <memory>

#include "roulette/random_generator.h"
#include "roulette/density_grid.h"
#include "roulette/three_tensor.h"
#include "roulette/photon.h"
#include "roulette/electron.h"
#include "roulette/distributions/exponential.h"
#include "roulette/distributions/compton_scattering.h"

namespace roulette {
  class DoseCalculation {
    private:
      std::shared_ptr<RandomGenerator> m_generator;
      std::shared_ptr<DensityGrid> m_density_grid;
      ThreeTensor m_dose;

      distributions::Exponential m_exponential_distribution;
      distributions::ComptonScattering m_compton_scattering;

    public:
      DoseCalculation(std::shared_ptr<RandomGenerator> generator, std::shared_ptr<DensityGrid> density_grid);

      const ThreeTensor& dose() const;

      void process_initial_photon(Photon photon);

      void process_photon(Photon& photon);
      void process_electron(Electron& electron);
  };
};
