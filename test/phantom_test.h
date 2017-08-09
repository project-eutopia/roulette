#pragma once

#include <memory>

#include "test_helper.h"

#include "roulette/density_compound_map.h"
#include "roulette/random_generator.h"
#include "roulette/phantom.h"
#include "roulette/photon.h"

using namespace roulette;

class PhantomRayTraceTest : public ::testing::Test {
  protected:
    std::shared_ptr<Phantom> phantom;

    PhantomRayTraceTest() {}
    virtual ~PhantomRayTraceTest() {}
    void SetUp() {
      VoxelGrid voxel_grid(
        ThreeVector(-5, -5, -5),
        ThreeVector( 5,  5,  5),
        1,1,1
      );

      const DensityCompoundMap map(compound_table.compound("Tissue, Soft (ICRU-44)"));

      phantom = std::make_shared<Phantom>(
        voxel_grid,
        ThreeTensor(4, 4, 2, 1.0)
      );
      phantom->set_compound_map(map);
    }

    // virtual void TearDown() {}
};

TEST_F(PhantomRayTraceTest, ray_trace_voxels_test_x_dir) {
  ThreeVector position(-10, 0, 0);
  ThreeVector velocity(1, 0, 0);

  std::vector<std::vector<int>> expected_voxels = {
    {0, 2, 1},
    {1, 2, 1},
    {2, 2, 1},
    {3, 2, 1}
  };
  int i = 0;

  ThreeVector final_position = phantom->ray_trace_voxels(
    position, velocity,
    Phantom::voxel_iterator(
      [&](const Phantom& phantom, double distance, int xi, int yi, int zi) -> double {
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

TEST_F(PhantomRayTraceTest, ray_trace_voxels_test_y_dir) {
  ThreeVector position(0, -10, 0);
  ThreeVector velocity(0, 1, 0);

  std::vector<std::vector<int>> expected_voxels = {
    {2, 0, 1},
    {2, 1, 1},
    {2, 2, 1},
    {2, 3, 1}
  };
  int i = 0;

  ThreeVector final_position = phantom->ray_trace_voxels(
    position, velocity,
    Phantom::voxel_iterator(
      [&](const Phantom& cur_phantom, double distance, int xi, int yi, int zi) -> double {
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

TEST_F(PhantomRayTraceTest, ray_trace_voxels_test_z_dir) {
  ThreeVector position(0, 0, 10);
  ThreeVector velocity(0, 0, -1);

  std::vector<std::vector<int>> expected_voxels = {
    {2, 2, 1},
    {2, 2, 0}
  };
  int i = 0;

  ThreeVector final_position = phantom->ray_trace_voxels(
    position, velocity,
    Phantom::voxel_iterator(
      [&](const Phantom& cur_phantom, double distance, int xi, int yi, int zi) -> double {
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

TEST_F(PhantomRayTraceTest, ray_trace_voxels_test_going_through_edge) {
  ThreeVector position(10, -5, 0);
  ThreeVector velocity(-2, 1, 0);

  std::vector<std::vector<int>> expected_voxels = {
    {3, 1, 1},
    {2, 1, 1},
    {1, 2, 1},
    {0, 2, 1}
  };
  int i = 0;

  ThreeVector final_position = phantom->ray_trace_voxels(
    position, velocity,
    Phantom::voxel_iterator(
      [&](const Phantom& cur_phantom, double distance, int xi, int yi, int zi) -> double {
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

TEST_F(PhantomRayTraceTest, ray_trace_voxels_test_going_through_corner) {
  ThreeVector position(-3, 3, -3);
  ThreeVector velocity(1, -1, 1);

  std::vector<std::vector<int>> expected_voxels = {
    {0, 3, 0},
    {1, 2, 0},
    {2, 1, 1},
    {3, 0, 1}
  };
  int i = 0;

  double distance_1 = std::sqrt(3*0.5*0.5);
  double distance_2 = std::sqrt(3*2.5*2.5);

  ThreeVector final_position = phantom->ray_trace_voxels(
    position, velocity,
    Phantom::voxel_iterator(
      [&](const Phantom& cur_phantom, double distance, int xi, int yi, int zi) -> double {
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

TEST(PhantomTest, ray_trace_voxels_going_through_random_directions) {
  VoxelGrid voxel_grid(
    ThreeVector(0, -10, -10),
    ThreeVector(10, 10, 10),
    1,1,1
  );

  const DensityCompoundMap map(compound_table.compound("Tissue, Soft (ICRU-44)"));

  Phantom phantom(
    voxel_grid,
    ThreeTensor(256, 1, 1, 1.0)
  );
  phantom.set_compound_map(map);

  ThreeVector start_position(-5, 0, 0);
  RandomGenerator generator;

  for (int i = 0; i < 5; ++i) {
    double theta = M_PI/4 * (generator.uniform()-0.5);
    double phi = 4*M_PI * (generator.uniform()-0.5);
    ThreeVector velocity(std::cos(theta), std::sin(theta)*std::cos(phi), std::sin(theta)*std::cos(phi));

    int count = 0;
    ThreeVector final_position = phantom.ray_trace_voxels(
      start_position, velocity,
      Phantom::voxel_iterator(
        [&](const Phantom& cur_phantom, double distance, int xi, int yi, int zi) -> double {
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

TEST(PhantomTest, transport_photon_unitless_depth_test) {
  const DensityCompoundMap map(compound_table.compound("Tissue, Soft (ICRU-44)"));

  VoxelGrid grid(ThreeVector(0, -10, -10), ThreeVector(200, 100, 100), 1, 1, 1);

  Phantom phantom(
    grid,
    ThreeTensor(256, 1, 1, 1.06)
  );
  phantom.set_compound_map(map);

  Photon initial_photon(FourMomentum(1000000, 1000000, 0, 0), ThreeVector(-10, 0, 0));
  bool res = phantom.transport_photon_unitless_depth(initial_photon, 1.0);

  EXPECT_TRUE(res);
  EXPECT_NEAR(1.0 / map.compound_for_density(1)->photon_total_cross_section(initial_photon.energy()) / 1.06, initial_photon.position()(0), 0.0001);
  EXPECT_EQ(0, initial_photon.position()(1));
  EXPECT_EQ(0, initial_photon.position()(2));
}

TEST(PhantomTest, transport_photon_unitless_depth_inhomogeneous_test) {
  const DensityCompoundMap map(compound_table.compound("Tissue, Soft (ICRU-44)"));

  VoxelGrid grid(ThreeVector(0, -25, -25), ThreeVector(50, 25, 25), 1, 1, 1);

  ThreeTensor densities(100, 1, 1, 1.0);
  for (int i = 20; i < 40; ++i) {
    densities(i, 0, 0) = 0.5;
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
