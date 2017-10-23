#pragma once

#include "test_helper.h"

#include "roulette/random_generator.h"
#include "roulette/regular_voxel_grid.h"
#include "roulette/photon.h"
#include "roulette/distributions/dirac_delta.h"
#include "roulette/beamlet_particle_generator.h"

using namespace roulette;

TEST(RegularVoxelGridTest, intersection_for_start_inside) {
  RegularVoxelGrid grid(
    ThreeVector(0, 0, 0),
    ThreeVector(10, 10, 20),
    1,1,1
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

TEST(RegularVoxelGridTest, intersection_for_start_on_plane) {
  RegularVoxelGrid grid(
    ThreeVector(0, 0, 0),
    ThreeVector(10, 10, 20),
    1,1,1
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

TEST(RegularVoxelGridTest, intersection_for_outside_grid) {
  RegularVoxelGrid grid(
    ThreeVector(0, 0, 0),
    ThreeVector(10, 10, 20),
    1,1,1
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

TEST(RegularVoxelGridTest, transport_particle_to_surface_works) {
  RegularVoxelGrid grid(
    ThreeVector(0, 0, 0),
    ThreeVector(10, 10, 20),
    1,1,1
  );

  Photon photon(
    FourMomentum(1, 1, 0, 0),
    ThreeVector(-5, 5, 10)
  );

  bool res = grid.transport_particle_to_surface(&photon);
  ASSERT_TRUE(res);

  ASSERT_EQ(photon.position()(0), 0);
}

TEST(RegularVoxelGridTest, transport_beamlet_particle_to_surface) {
  RandomGenerator generator;
  RegularVoxelGrid grid(ThreeVector(0, -10, -10), ThreeVector(20, 10, 10), 1, 1, 1);
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

TEST(RegularVoxelGridTest, ray_trace_voxels_test_x_dir) {
  ThreeVector position(-10, 0, 0);
  ThreeVector velocity(1, 0, 0);

  std::shared_ptr<const RegularVoxelGrid> voxel_grid = std::make_shared<const RegularVoxelGrid>(
    ThreeVector(-5, -5, -5),
    ThreeVector( 5,  5,  5),
    4,4,2
  );

  std::vector<std::vector<int>> expected_voxels = {
    {0, 2, 1},
    {1, 2, 1},
    {2, 2, 1},
    {3, 2, 1}
  };
  int i = 0;

  ThreeVector final_position = voxel_grid->ray_trace_voxels(
    position, velocity,
    IVoxelGrid::voxel_iterator(
      [&](double distance, size_t xi, size_t yi, size_t zi) -> double {
        EXPECT_EQ(xi, expected_voxels[i][0]);
        EXPECT_EQ(yi, expected_voxels[i][1]);
        EXPECT_EQ(zi, expected_voxels[i][2]);
        EXPECT_EQ(distance, 2.5);
        i++;

        return distance;
      }
    )
  );

  EXPECT_EQ(i, 4);
  EXPECT_EQ(final_position(0), 5);
  EXPECT_EQ(final_position(1), 0);
  EXPECT_EQ(final_position(2), 0);
}

TEST(RegularVoxelGridTest, ray_trace_voxels_test_y_dir) {
  ThreeVector position(0, -10, 0);
  ThreeVector velocity(0, 1, 0);

  std::shared_ptr<const RegularVoxelGrid> voxel_grid = std::make_shared<const RegularVoxelGrid>(
    ThreeVector(-5, -5, -5),
    ThreeVector( 5,  5,  5),
    4,4,2
  );

  std::vector<std::vector<int>> expected_voxels = {
    {2, 0, 1},
    {2, 1, 1},
    {2, 2, 1},
    {2, 3, 1}
  };
  int i = 0;

  ThreeVector final_position = voxel_grid->ray_trace_voxels(
    position, velocity,
    IVoxelGrid::voxel_iterator(
      [&](double distance, int xi, int yi, int zi) -> double {
        EXPECT_EQ(xi, expected_voxels[i][0]);
        EXPECT_EQ(yi, expected_voxels[i][1]);
        EXPECT_EQ(zi, expected_voxels[i][2]);
        EXPECT_EQ(distance, 2.5);
        i++;

        return distance;
      }
    )
  );

  EXPECT_EQ(i, 4);
  EXPECT_EQ(final_position(0), 0);
  EXPECT_EQ(final_position(1), 5);
  EXPECT_EQ(final_position(2), 0);
}

TEST(RegularVoxelGridTest, ray_trace_voxels_test_z_dir) {
  ThreeVector position(0, 0, 10);
  ThreeVector velocity(0, 0, -1);

  std::shared_ptr<const RegularVoxelGrid> voxel_grid = std::make_shared<const RegularVoxelGrid>(
    ThreeVector(-5, -5, -5),
    ThreeVector( 5,  5,  5),
    4,4,2
  );

  std::vector<std::vector<int>> expected_voxels = {
    {2, 2, 1},
    {2, 2, 0}
  };
  int i = 0;

  ThreeVector final_position = voxel_grid->ray_trace_voxels(
    position, velocity,
    IVoxelGrid::voxel_iterator(
      [&](double distance, size_t xi, size_t yi, size_t zi) -> double {
        EXPECT_EQ(xi, expected_voxels[i][0]);
        EXPECT_EQ(yi, expected_voxels[i][1]);
        EXPECT_EQ(zi, expected_voxels[i][2]);
        EXPECT_EQ(distance, 5);
        i++;

        return distance;
      }
    )
  );

  EXPECT_EQ(i, 2);
  EXPECT_EQ(final_position(0), 0);
  EXPECT_EQ(final_position(1), 0);
  EXPECT_EQ(final_position(2),-5);
}

TEST(RegularVoxelGridTest, ray_trace_voxels_test_going_through_edge) {
  ThreeVector position(10, -5, 0);
  ThreeVector velocity(-2, 1, 0);

  std::shared_ptr<const RegularVoxelGrid> voxel_grid = std::make_shared<const RegularVoxelGrid>(
    ThreeVector(-5, -5, -5),
    ThreeVector( 5,  5,  5),
    4,4,2
  );

  std::vector<std::vector<int>> expected_voxels = {
    {3, 1, 1},
    {2, 1, 1},
    {1, 2, 1},
    {0, 2, 1}
  };
  int i = 0;

  ThreeVector final_position = voxel_grid->ray_trace_voxels(
    position, velocity,
    IVoxelGrid::voxel_iterator(
      [&](double distance, size_t xi, size_t yi, size_t zi) -> double {
        EXPECT_EQ(xi, expected_voxels[i][0]);
        EXPECT_EQ(yi, expected_voxels[i][1]);
        EXPECT_EQ(zi, expected_voxels[i][2]);
        EXPECT_EQ(distance, std::sqrt(2.5*2.5 + 1.25*1.25));
        i++;

        return distance;
      }
    )
  );

  EXPECT_EQ(i, 4);
  EXPECT_EQ(final_position(0),  -5);
  EXPECT_EQ(final_position(1), 2.5);
  EXPECT_EQ(final_position(2),   0);
}

TEST(RegularVoxelGridTest, ray_trace_voxels_test_going_through_corner) {
  ThreeVector position(-3, 3, -3);
  ThreeVector velocity(1, -1, 1);

  std::shared_ptr<const RegularVoxelGrid> voxel_grid = std::make_shared<const RegularVoxelGrid>(
    ThreeVector(-5, -5, -5),
    ThreeVector( 5,  5,  5),
    4,4,2
  );

  std::vector<std::vector<int>> expected_voxels = {
    {0, 3, 0},
    {1, 2, 0},
    {2, 1, 1},
    {3, 0, 1}
  };
  int i = 0;

  double distance_1 = std::sqrt(3*0.5*0.5);
  double distance_2 = std::sqrt(3*2.5*2.5);

  ThreeVector final_position = voxel_grid->ray_trace_voxels(
    position, velocity,
    IVoxelGrid::voxel_iterator(
      [&](double distance, size_t xi, size_t yi, size_t zi) -> double {
        EXPECT_EQ(xi, expected_voxels[i][0]);
        EXPECT_EQ(yi, expected_voxels[i][1]);
        EXPECT_EQ(zi, expected_voxels[i][2]);
        if (i == 0) {
          EXPECT_NEAR(distance, distance_1, 0.0000001);
        }
        else {
          EXPECT_NEAR(distance, distance_2, 0.0000001);
        }
        i++;

        return distance;
      }
    )
  );

  EXPECT_EQ(i, 4);
  EXPECT_EQ(final_position(0),  5);
  EXPECT_EQ(final_position(1), -5);
  EXPECT_EQ(final_position(2),  5);
}

TEST(RegularVoxelGridTest, ray_trace_voxels_going_through_random_directions) {
  std::shared_ptr<const RegularVoxelGrid> voxel_grid = std::make_shared<const RegularVoxelGrid>(
    ThreeVector(0, -10, -10),
    ThreeVector(10, 10, 10),
    256,1,1
  );

  ThreeVector start_position(-5, 0, 0);
  RandomGenerator generator;

  for (int i = 0; i < 5; ++i) {
    double theta = M_PI/4 * (generator.uniform()-0.5);
    double phi = 4*M_PI * (generator.uniform()-0.5);
    ThreeVector velocity(std::cos(theta), std::sin(theta)*std::cos(phi), std::sin(theta)*std::cos(phi));

    int count = 0;
    ThreeVector final_position = voxel_grid->ray_trace_voxels(
      start_position, velocity,
      IVoxelGrid::voxel_iterator(
        [&](double distance, size_t xi, size_t yi, size_t zi) -> double {
          EXPECT_EQ(xi, count);
          ++count;

          return distance;
        }
      )
    );

    EXPECT_EQ(count, 256);
    EXPECT_NEAR(final_position(0), 10, 0.0000001);
  }
}
