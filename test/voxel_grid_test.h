#pragma once

#include "test_helper.h"

#include "roulette/voxel_grid.h"
#include "roulette/photon.h"

using namespace roulette;

TEST(VoxelGridTest, intersection_times_for_start_on) {
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

    std::pair<double,double> t = grid.intersection_times(photon);

    ASSERT_EQ(t.first, 5);
    ASSERT_TRUE(std::isnan(t.second));
  }

  {
    Photon photon(
      FourMomentum(1, 2, 2, 0),
      ThreeVector(5, 5, 10)
    );
    ASSERT_TRUE(grid.inside(photon.position()));

    std::pair<double,double> t = grid.intersection_times(photon);

    ASSERT_EQ(t.first, 2.5);
    ASSERT_TRUE(std::isnan(t.second));
  }

  {
    Photon photon(
      FourMomentum(1, 0.5, 0.5, -1),
      ThreeVector(5, 5, 10)
    );
    ASSERT_TRUE(grid.inside(photon.position()));

    std::pair<double,double> t = grid.intersection_times(photon);

    ASSERT_EQ(t.first, 10);
    ASSERT_TRUE(std::isnan(t.second));
  }
}

TEST(VoxelGridTest, intersection_times_for_start_on_plane) {
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

    std::pair<double,double> t = grid.intersection_times(photon);

    ASSERT_EQ(t.first, 0);
    ASSERT_EQ(t.second, 10);
  }

  {
    Photon photon(
      FourMomentum(1, 1, 0, 0),
      ThreeVector(0, 0, 10)
    );
    ASSERT_TRUE(grid.inside(photon.position()));

    std::pair<double,double> t = grid.intersection_times(photon);

    ASSERT_EQ(t.first, 0);
    ASSERT_EQ(t.second, 10);
  }

  {
    Photon photon(
      FourMomentum(1, 1, 0, 0),
      ThreeVector(0, 0, 0)
    );
    ASSERT_TRUE(grid.inside(photon.position()));

    std::pair<double,double> t = grid.intersection_times(photon);

    ASSERT_EQ(t.first, 0);
    ASSERT_EQ(t.second, 10);
  }
}

TEST(VoxelGridTest, intersection_times_for_outside_grid) {
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

    std::pair<double,double> t = grid.intersection_times(photon);

    ASSERT_EQ(t.first, 5);
    ASSERT_EQ(t.second, 15);
  }

  {
    Photon photon(
      FourMomentum(1, 1, 0, 0),
      ThreeVector(-5, 0, 10)
    );
    ASSERT_FALSE(grid.inside(photon.position()));

    std::pair<double,double> t = grid.intersection_times(photon);

    ASSERT_EQ(t.first, 5);
    ASSERT_EQ(t.second, 15);
  }

  {
    Photon photon(
      FourMomentum(1, 1, 0, 0),
      ThreeVector(-5, 0, 0)
    );
    ASSERT_FALSE(grid.inside(photon.position()));

    std::pair<double,double> t = grid.intersection_times(photon);

    ASSERT_EQ(t.first, 5);
    ASSERT_EQ(t.second, 15);
  }

  {
    Photon photon(
      FourMomentum(1, 1, 0, 0),
      ThreeVector(-5, -5, -5)
    );
    ASSERT_FALSE(grid.inside(photon.position()));

    std::pair<double,double> t = grid.intersection_times(photon);

    ASSERT_TRUE(std::isnan(t.first));
    ASSERT_TRUE(std::isnan(t.second));
  }

  {
    Photon photon(
      FourMomentum(1, 1, 1, 0),
      ThreeVector(-5, 5, 10)
    );
    ASSERT_FALSE(grid.inside(photon.position()));

    std::pair<double,double> t = grid.intersection_times(photon);

    ASSERT_EQ(t.first, 5);
    ASSERT_TRUE(std::isnan(t.second));
  }
}
