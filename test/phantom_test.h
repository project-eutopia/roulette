#pragma once

#include <memory>

#include "test_helper.h"

#include "roulette/density_compound_map.h"
#include "roulette/random_generator.h"
#include "roulette/phantom.h"
#include "roulette/photon.h"
#include "roulette/matrix_three_tensor.h"

using namespace roulette;

TEST(PhantomTest, transport_photon_unitless_depth_inhomogeneous_test) {
  const DensityCompoundMap map(compound_table.compound("Tissue, Soft (ICRU-44)"));

  std::shared_ptr<const RegularVoxelGrid> grid = std::make_shared<const RegularVoxelGrid>(
    ThreeVector(0, -25, -25), ThreeVector(50, 25, 25), 100, 1, 1
  );

  std::shared_ptr<MatrixThreeTensor> densities = std::make_shared<MatrixThreeTensor>(100, 1, 1, 1.0);
  for (int i = 20; i < 40; ++i) {
    (*densities)(i, 0, 0) = 0.5;
  }

  Phantom phantom(
    grid,
    densities
  );
  phantom.set_compound_map(map);

  Photon initial_photon(FourMomentum(1000000, 1000000, 0, 0), ThreeVector(-10, 0, 0));
  double cross_section = map.compound_for_density(1)->photon_scattering_cross_section(initial_photon.energy()) + map.compound_for_density(1)->photon_absorption_cross_section(initial_photon.energy());

  phantom.transport_photon_unitless_depth(initial_photon, 2.5 * cross_section);
  EXPECT_NEAR(2.5, initial_photon.position()(0), 0.0001);
  phantom.transport_photon_unitless_depth(initial_photon, 7.4 * cross_section);
  EXPECT_NEAR(9.9, initial_photon.position()(0), 0.0001);
  phantom.transport_photon_unitless_depth(initial_photon, 0.1 * cross_section);
  EXPECT_NEAR(10.0, initial_photon.position()(0), 0.0001);
  phantom.transport_photon_unitless_depth(initial_photon, 0.1 * cross_section);
  EXPECT_NEAR(10.2, initial_photon.position()(0), 0.0001);
  phantom.transport_photon_unitless_depth(initial_photon, 4.8 * cross_section);
  EXPECT_NEAR(19.8, initial_photon.position()(0), 0.0001);
  phantom.transport_photon_unitless_depth(initial_photon, 0.1 * cross_section);
  EXPECT_NEAR(20.0, initial_photon.position()(0), 0.0001);
  phantom.transport_photon_unitless_depth(initial_photon, 0.1 * cross_section);
  EXPECT_NEAR(20.1, initial_photon.position()(0), 0.0001);
  phantom.transport_photon_unitless_depth(initial_photon, 4.9 * cross_section);
  EXPECT_NEAR(25.0, initial_photon.position()(0), 0.0001);
}
