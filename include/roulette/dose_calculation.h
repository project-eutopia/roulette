#pragma once

#include <memory>

#include "roulette/random_generator.h"
#include "roulette/density_grid.h"
#include "roulette/three_tensor.h"
#include "roulette/photon.h"
#include "roulette/electron.h"
#include "roulette/interaction_function.h"
#include "roulette/distributions/exponential.h"
#include "roulette/distributions/compton_scattering.h"
#include "roulette/distributions/spherical.h"

namespace roulette {
  class DoseCalculation {
    private:
      std::shared_ptr<RandomGenerator> m_generator;
      std::shared_ptr<DensityGrid> m_density_grid;
      ThreeTensor m_dose;

      distributions::Exponential m_exponential_distribution;
      distributions::ComptonScattering m_compton_scattering;
      distributions::Spherical m_spherical;

      InteractionFunction m_interaction_function;

    public:
      enum InteractionType { NONE, PHOTON_SCATTER, PHOTON_PHOTOELECTRIC, ELECTRON_ABSORB };
      // type, energy, weight
      std::vector<std::tuple<InteractionType,FourMomentum,ThreeVector>> interactions;

      DoseCalculation(std::shared_ptr<RandomGenerator> generator, std::shared_ptr<DensityGrid> density_grid);

      const ThreeTensor& dose() const;

      void process_initial_photon(Photon photon);

      void process_photon(Photon& photon);
      void process_electron(Electron& electron);
  };
};
