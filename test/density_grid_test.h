#pragma once

#include <memory>

#include "test_helper.h"

#include "roulette/density_grid.h"

using namespace roulette;

class DensityGridTest : public ::testing::Test {
  protected:
    std::shared_ptr<DensityGrid> density_grid;

    DensityGridTest() {}
    virtual ~DensityGridTest() {}
    void SetUp() {
      VoxelGrid voxel_grid(
        ThreeVector(-5, -5, -5),
        ThreeVector( 5,  5,  5)
      );

      Material soft_tissue_icru_44(
        1.060E+00,
        NonUniformLinearInterpolation("../data/soft_tissue_icru_44.txt")
      );

      density_grid = std::make_shared<DensityGrid>(
        voxel_grid,
        ThreeTensor(4, 4, 2, 1.0),
        soft_tissue_icru_44
      );
    }

    // virtual void TearDown() {}
};

TEST_F(DensityGridTest, ray_trace_voxels_test_x_dir) {
  ThreeVector position(-10, 0, 0);
  ThreeVector velocity(1, 0, 0);

  std::vector<std::vector<int>> expected_voxels = {
    {0, 2, 1},
    {1, 2, 1},
    {2, 2, 1},
    {3, 2, 1}
  };
  int i = 0;

  ThreeVector final_position = density_grid->ray_trace_voxels(
    position, velocity,
    DensityGrid::voxel_iterator(
      [&](const DensityGrid& cur_density_grid, double distance, int xi, int yi, int zi) -> double {
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

TEST_F(DensityGridTest, ray_trace_voxels_test_y_dir) {
  ThreeVector position(0, -10, 0);
  ThreeVector velocity(0, 1, 0);

  std::vector<std::vector<int>> expected_voxels = {
    {2, 0, 1},
    {2, 1, 1},
    {2, 2, 1},
    {2, 3, 1}
  };
  int i = 0;

  ThreeVector final_position = density_grid->ray_trace_voxels(
    position, velocity,
    DensityGrid::voxel_iterator(
      [&](const DensityGrid& cur_density_grid, double distance, int xi, int yi, int zi) -> double {
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

TEST_F(DensityGridTest, ray_trace_voxels_test_z_dir) {
  ThreeVector position(0, 0, 10);
  ThreeVector velocity(0, 0, -1);

  std::vector<std::vector<int>> expected_voxels = {
    {2, 2, 1},
    {2, 2, 0}
  };
  int i = 0;

  ThreeVector final_position = density_grid->ray_trace_voxels(
    position, velocity,
    DensityGrid::voxel_iterator(
      [&](const DensityGrid& cur_density_grid, double distance, int xi, int yi, int zi) -> double {
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

TEST_F(DensityGridTest, ray_trace_voxels_test_going_through_edge) {
  ThreeVector position(10, -5, 0);
  ThreeVector velocity(-2, 1, 0);

  std::vector<std::vector<int>> expected_voxels = {
    {3, 1, 1},
    {2, 1, 1},
    {1, 2, 1},
    {0, 2, 1}
  };
  int i = 0;

  ThreeVector final_position = density_grid->ray_trace_voxels(
    position, velocity,
    DensityGrid::voxel_iterator(
      [&](const DensityGrid& cur_density_grid, double distance, int xi, int yi, int zi) -> double {
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

TEST_F(DensityGridTest, ray_trace_voxels_test_going_through_corner) {
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

  ThreeVector final_position = density_grid->ray_trace_voxels(
    position, velocity,
    DensityGrid::voxel_iterator(
      [&](const DensityGrid& cur_density_grid, double distance, int xi, int yi, int zi) -> double {
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
