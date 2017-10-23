#pragma once

#include "test_helper.h"

#include "roulette/random_generator.h"
#include "roulette/irregular_voxel_grid.h"
#include "roulette/photon.h"
#include "roulette/distributions/dirac_delta.h"
#include "roulette/beamlet_particle_generator.h"

using namespace roulette;

TEST(IrregularVoxelGridTest, intersection_for_start_inside) {
  IrregularVoxelGrid grid(
    {0, 10},
    {0, 10},
    {0, 20}
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

TEST(IrregularVoxelGridTest, intersection_for_start_on_plane) {
  IrregularVoxelGrid grid(
    {0, 10},
    {0, 10},
    {0, 20}
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

TEST(IrregularVoxelGridTest, intersection_for_outside_grid) {
  IrregularVoxelGrid grid(
    {0, 10},
    {0, 10},
    {0, 20}
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

TEST(IrregularVoxelGridTest, transport_particle_to_surface_works) {
  IrregularVoxelGrid grid(
    {0, 10},
    {0, 10},
    {0, 20}
  );

  Photon photon(
    FourMomentum(1, 1, 0, 0),
    ThreeVector(-5, 5, 10)
  );

  bool res = grid.transport_particle_to_surface(&photon);
  ASSERT_TRUE(res);

  ASSERT_EQ(photon.position()(0), 0);
}

TEST(IrregularVoxelGridTest, transport_beamlet_particle_to_surface) {
  IrregularVoxelGrid grid(
    {0, 20},
    {-10, 10},
    {-10, 20}
  );

  RandomGenerator generator;
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

TEST(IrregularVoxelGridTest, ray_trace_voxels_test_x_dir) {
  ThreeVector position(-10, 0, 0);
  ThreeVector velocity(1, 0, 0);

  IrregularVoxelGrid grid(
    {-5, -4, 0, 4, 5},
    {-5, -4, 0, 4, 5},
    {-5, 0, 5}
  );

  std::vector<std::vector<int>> expected_voxels = {
    {0, 2, 1},
    {1, 2, 1},
    {2, 2, 1},
    {3, 2, 1}
  };

  int i = 0;

  ThreeVector final_position = grid.ray_trace_voxels(
    position, velocity,
    IVoxelGrid::voxel_iterator(
      [&](double distance, size_t xi, size_t yi, size_t zi) -> double {
        EXPECT_EQ(xi, expected_voxels[i][0]);
        EXPECT_EQ(yi, expected_voxels[i][1]);
        EXPECT_EQ(zi, expected_voxels[i][2]);

        if (i == 0 || i == 3) {
          EXPECT_EQ(distance, 1.0);
        }
        else {
          EXPECT_EQ(distance, 4.0);
        }
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

TEST(IrregularVoxelGridTest, ray_trace_voxels_test_y_dir) {
  IrregularVoxelGrid grid(
    {-5, -4, 0, 4, 5},
    {-5, -4, 0, 4, 5},
    {-5, 0, 5}
  );

  ThreeVector position(0, -10, 0);
  ThreeVector velocity(0, 1, 0);

  std::vector<std::vector<int>> expected_voxels = {
    {2, 0, 1},
    {2, 1, 1},
    {2, 2, 1},
    {2, 3, 1}
  };
  int i = 0;

  ThreeVector final_position = grid.ray_trace_voxels(
    position, velocity,
    IVoxelGrid::voxel_iterator(
      [&](double distance, size_t xi, size_t yi, size_t zi) -> double {
        EXPECT_EQ(xi, expected_voxels[i][0]);
        EXPECT_EQ(yi, expected_voxels[i][1]);
        EXPECT_EQ(zi, expected_voxels[i][2]);

        if (i == 0 || i == 3) {
          EXPECT_EQ(distance, 1.0);
        }
        else {
          EXPECT_EQ(distance, 4.0);
        }
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

TEST(IrregularVoxelGridTest, ray_trace_voxels_test_z_dir) {
  IrregularVoxelGrid grid(
    {-5, -4, 0, 4, 5},
    {-5, -4, 0, 4, 5},
    {-5, 1, 5}
  );

  ThreeVector position(0, 0, 10);
  ThreeVector velocity(0, 0, -1);

  std::vector<std::vector<int>> expected_voxels = {
    {2, 2, 1},
    {2, 2, 0}
  };
  int i = 0;

  ThreeVector final_position = grid.ray_trace_voxels(
    position, velocity,
    IVoxelGrid::voxel_iterator(
      [&](double distance, size_t xi, size_t yi, size_t zi) -> double {
        EXPECT_EQ(xi, expected_voxels[i][0]);
        EXPECT_EQ(yi, expected_voxels[i][1]);
        EXPECT_EQ(zi, expected_voxels[i][2]);
        if (i == 0) {
          EXPECT_EQ(distance, 4);
        }
        else {
          EXPECT_EQ(distance, 6);
        }
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

TEST(IrregularVoxelGridTest, ray_trace_voxels_test_going_through_edge) {
  IrregularVoxelGrid grid(
    {-5, -4, 0, 4, 5},
    {-5, -4, 0, 4, 5},
    {-5, 0, 5}
  );

  ThreeVector position(10, -5, 0);
  ThreeVector velocity(-2, 1, 0);

  std::vector<std::vector<int>> expected_voxels = {
    {3, 1, 1},
    {2, 1, 1},
    {1, 2, 1},
    {0, 2, 1}
  };
  int i = 0;

  ThreeVector final_position = grid.ray_trace_voxels(
    position, velocity,
    IVoxelGrid::voxel_iterator(
      [&](double distance, size_t xi, size_t yi, size_t zi) -> double {
        EXPECT_EQ(xi, expected_voxels[i][0]);
        EXPECT_EQ(yi, expected_voxels[i][1]);
        EXPECT_EQ(zi, expected_voxels[i][2]);

        if (i == 0 || i == 3) {
          EXPECT_EQ(distance, std::sqrt(5)/2);
        }
        else {
          EXPECT_EQ(distance, 2*std::sqrt(5));
        }
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

TEST(IrregularVoxelGridTest, ray_trace_voxels_test_going_through_corner) {
  IrregularVoxelGrid grid(
    {-5, -4, 0, 4, 5},
    {-5, -4, 0, 4, 5},
    {-5, 0, 5}
  );

  ThreeVector position(-3, 3, -3);
  ThreeVector velocity(1, -1, 1);

  std::vector<std::vector<int>> expected_voxels = {
    {1, 2, 0},
    {2, 1, 1},
    {3, 0, 1}
  };
  int i = 0;

  ThreeVector final_position = grid.ray_trace_voxels(
    position, velocity,
    IVoxelGrid::voxel_iterator(
      [&](double distance, size_t xi, size_t yi, size_t zi) -> double {
        EXPECT_EQ(xi, expected_voxels[i][0]);
        EXPECT_EQ(yi, expected_voxels[i][1]);
        EXPECT_EQ(zi, expected_voxels[i][2]);
        if (i == 0) {
          EXPECT_NEAR(distance, 3*std::sqrt(3), 0.0000001);
        }
        else if (i == 1) {
          EXPECT_NEAR(distance, 4*std::sqrt(3), 0.0000001);
        }
        else {
          EXPECT_NEAR(distance, std::sqrt(3), 0.0000001);
        }
        i++;

        return distance;
      }
    )
  );

  EXPECT_EQ(i, 3);
  EXPECT_EQ(final_position(0),  5);
  EXPECT_EQ(final_position(1), -5);
  EXPECT_EQ(final_position(2),  5);
}

TEST(IrregularVoxelGridTest, ray_trace_voxels_going_through_random_directions) {
  IrregularVoxelGrid grid(
    {0, 0.5, 1, 1.5, 2, 2.5, 3, 3.5, 4, 4.5, 5, 5.5, 6, 6.5, 7, 7.5, 8, 8.5, 9, 9.5, 10},
    {-10, 10},
    {-10, 10}
  );

  ThreeVector start_position(-5, 0, 0);
  RandomGenerator generator;

  for (int i = 0; i < 5; ++i) {
    double theta = M_PI/4 * (generator.uniform()-0.5);
    double phi = 4*M_PI * (generator.uniform()-0.5);
    ThreeVector velocity(std::cos(theta), std::sin(theta)*std::cos(phi), std::sin(theta)*std::cos(phi));

    int count = 0;
    ThreeVector final_position = grid.ray_trace_voxels(
      start_position, velocity,
      IVoxelGrid::voxel_iterator(
        [&](double distance, size_t xi, size_t yi, size_t zi) -> double {
          EXPECT_EQ(xi, count);
          ++count;

          return distance;
        }
      )
    );

    EXPECT_EQ(count, 20);
    EXPECT_NEAR(final_position(0), 10, 0.0000001);
  }
}
