#pragma once

#include "test_helper.h"

#include "roulette/electron.h"

using namespace roulette;

TEST(ElectronTest, kinetic_energy_test) {
  double energy1 = 1000000;
  double mom1 = std::sqrt(energy1*energy1 - Electron::MASS*Electron::MASS);
  Electron e1(energy1, mom1, ThreeVector(1, 0, 0), ThreeVector(0, 0, 0));

  EXPECT_EQ(e1.kinetic_energy(), 1000000 - Electron::MASS);
}
