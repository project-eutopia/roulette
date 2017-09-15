#pragma once

#include "test_helper.h"

#include "roulette/four_momentum.h"
#include "roulette/rotation_matrix.h"

using namespace roulette;

TEST(FourMomentum, can_be_rotated) {
  FourMomentum m(1, 1, 0, 0);
  RotationMatrix rot = RotationMatrix::rotationZ(M_PI/4);

  FourMomentum rot_m = rot * m;

  ASSERT_EQ(rot_m(0), 1);
  ASSERT_NEAR(rot_m(1), 1/std::sqrt(2), 0.0000001);
  ASSERT_NEAR(rot_m(2), 1/std::sqrt(2), 0.0000001);
  ASSERT_EQ(rot_m(3), 0);
}
