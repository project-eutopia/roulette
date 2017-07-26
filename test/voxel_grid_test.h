#pragma once

#include "test_helper.h"

#include "roulette/random_generator.h"
#include "roulette/voxel_grid.h"
#include "roulette/photon.h"
#include "roulette/distributions/dirac_delta.h"
#include "roulette/beamlet_particle_generator.h"

using namespace roulette;

TEST(VoxelGridTest, intersection_for_start_inside) {
  VoxelGrid grid(
    ThreeVector(0, 0, 0),
    ThreeVector(10, 10, 20)
  );

  {
    Photon photon(
      FourMomentum(1, 1, 0, 0),
      ThreeVector(5, 5, 10)
    );
    ASSERT_TRUE(grid.inside(photon.position()));

    bool res = grid.transport_particle_to_surface(&photon);
    ASSERT_TRUE(res);
    ASSERT_EQ(photon.position()(0), 10);
    ASSERT_EQ(photon.position()(1), 5);
    ASSERT_EQ(photon.position()(2), 10);
  }

  {
    Photon photon(
      FourMomentum(1, 2, 2, 0),
      ThreeVector(5, 5, 10)
    );
    ASSERT_TRUE(grid.inside(photon.position()));

    bool res = grid.transport_particle_to_surface(&photon);
    ASSERT_TRUE(res);
    ASSERT_EQ(photon.position()(0), 10);
    ASSERT_EQ(photon.position()(1), 10);
    ASSERT_EQ(photon.position()(2), 10);
  }

  {
    Photon photon(
      FourMomentum(1, 0.5, 0.5, -1),
      ThreeVector(5, 5, 10)
    );
    ASSERT_TRUE(grid.inside(photon.position()));

    bool res = grid.transport_particle_to_surface(&photon);
    ASSERT_TRUE(res);
    ASSERT_EQ(photon.position()(0), 10);
    ASSERT_EQ(photon.position()(1), 10);
    ASSERT_EQ(photon.position()(2), 0);
  }
}

TEST(VoxelGridTest, intersection_for_start_on_plane) {
  VoxelGrid grid(
    ThreeVector(0, 0, 0),
    ThreeVector(10, 10, 20)
  );

  {
    Photon photon(
      FourMomentum(1, 1, 0, 0),
      ThreeVector(0, 5, 10)
    );
    ASSERT_TRUE(grid.inside(photon.position()));

    bool res = grid.transport_particle_to_surface(&photon);
    ASSERT_TRUE(res);
    ASSERT_EQ(photon.position()(0), 0);
    ASSERT_EQ(photon.position()(1), 5);
    ASSERT_EQ(photon.position()(2), 10);
  }

  {
    Photon photon(
      FourMomentum(1, 1, 0, 0),
      ThreeVector(0, 0, 10)
    );
    ASSERT_TRUE(grid.inside(photon.position()));

    bool res = grid.transport_particle_to_surface(&photon);
    ASSERT_TRUE(res);
    ASSERT_EQ(photon.position()(0), 0);
    ASSERT_EQ(photon.position()(1), 0);
    ASSERT_EQ(photon.position()(2), 10);
  }

  {
    Photon photon(
      FourMomentum(1, 1, 0, 0),
      ThreeVector(0, 0, 0)
    );
    ASSERT_TRUE(grid.inside(photon.position()));

    bool res = grid.transport_particle_to_surface(&photon);
    ASSERT_TRUE(res);
    ASSERT_EQ(photon.position()(0), 0);
    ASSERT_EQ(photon.position()(1), 0);
    ASSERT_EQ(photon.position()(2), 0);
  }
}

TEST(VoxelGridTest, intersection_for_outside_grid) {
  VoxelGrid grid(
    ThreeVector(0, 0, 0),
    ThreeVector(10, 10, 20)
  );

  {
    Photon photon(
      FourMomentum(1, 1, 0, 0),
      ThreeVector(-5, 5, 10)
    );
    ASSERT_FALSE(grid.inside(photon.position()));

    bool res = grid.transport_particle_to_surface(&photon);
    ASSERT_TRUE(res);
    ASSERT_EQ(photon.position()(0), 0);
    ASSERT_EQ(photon.position()(1), 5);
    ASSERT_EQ(photon.position()(2), 10);
  }

  {
    Photon photon(
      FourMomentum(1, 1, 0, 0),
      ThreeVector(-5, 0, 10)
    );
    ASSERT_FALSE(grid.inside(photon.position()));

    bool res = grid.transport_particle_to_surface(&photon);
    ASSERT_FALSE(res);
  }

  {
    Photon photon(
      FourMomentum(1, 1, 0, 0),
      ThreeVector(-5, 0, 0)
    );
    ASSERT_FALSE(grid.inside(photon.position()));

    bool res = grid.transport_particle_to_surface(&photon);
    ASSERT_FALSE(res);
  }

  {
    Photon photon(
      FourMomentum(1, 1, 0, 0),
      ThreeVector(-5, -5, -5)
    );
    ASSERT_FALSE(grid.inside(photon.position()));

    bool res = grid.transport_particle_to_surface(&photon);
    ASSERT_FALSE(res);
  }

  {
    Photon photon(
      FourMomentum(1, 1, 1, 0),
      ThreeVector(-5, 5, 10)
    );
    ASSERT_FALSE(grid.inside(photon.position()));

    bool res = grid.transport_particle_to_surface(&photon);
    ASSERT_FALSE(res);
  }
}

TEST(VoxelGridTest, transport_particle_to_surface_works) {
  VoxelGrid grid(
    ThreeVector(0, 0, 0),
    ThreeVector(10, 10, 20)
  );

  Photon photon(
    FourMomentum(1, 1, 0, 0),
    ThreeVector(-5, 5, 10)
  );

  bool res = grid.transport_particle_to_surface(&photon);
  ASSERT_TRUE(res);

  ASSERT_EQ(photon.position()(0), 0);
}

TEST(VoxelGridTest, transport_beamlet_particle_to_surface) {
  RandomGenerator generator;
  VoxelGrid grid(ThreeVector(0, -10, -10), ThreeVector(20, 10, 10));
  Beamlet beamlet(ThreeVector(-10, 0, 0), ThreeVector(0, -1, -1), ThreeVector(0, 1, -1), ThreeVector(0, 1, 1));
  double energy = 1000000;
  BeamletParticleGenerator<Photon,distributions::DiracDelta> photon_generator(
    beamlet,
    distributions::DiracDelta(energy)
  );

  Photon initial_photon = photon_generator.generate(generator);
  Photon current_photon = initial_photon;
  bool res = grid.transport_particle_to_surface(&current_photon);
  ASSERT_TRUE(res);
}
