#pragma once

#include "test_helper.h"

#include "roulette/photon.h"
#include "roulette/distributions/compton_scattering.h"

using namespace roulette;

TEST(PhotonTest, compton_scatters) {
  RandomGenerator generator;
  distributions::ComptonScattering compton;

  double energy = 6000000;
  Photon photon(
    FourMomentum(energy, energy, 0, 0),
    ThreeVector(0, 0, 0)
  );
  Electron electron(
    FourMomentum(Electron::MASS, 0, 0, 0),
    ThreeVector(0, 0, 0)
  );

  for (int i = 0; i < 5; ++i) {
    Photon current_photon = photon;

    compton.set_initial_photon(current_photon);
    compton(generator);

    Electron current_electron = current_photon.compton_scatter(
      ELECTRON_REST_ENERGY_IN_EV * compton.final_photon_energy(),
      ELECTRON_REST_ENERGY_IN_EV * compton.final_electron_energy(),
      compton.final_photon_theta(),
      compton.final_electron_theta(),
      compton.final_phi()
    );

    double error = 0.0001;

    FourMomentum initial_momentum = photon.momentum() + electron.momentum();
    FourMomentum final_momentum = current_photon.momentum() + current_electron.momentum();

    ASSERT_NEAR(initial_momentum(0), final_momentum(0), error);
    ASSERT_NEAR(initial_momentum(1), final_momentum(1), error);
    ASSERT_NEAR(initial_momentum(2), final_momentum(2), error);
    ASSERT_NEAR(initial_momentum(3), final_momentum(3), error);
  }
}
