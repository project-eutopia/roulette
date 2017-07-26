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

TEST_F(DensityGridTest, ray_trace_voxels_test) {
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
