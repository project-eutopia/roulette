#pragma once

#include "test_helper.h"
#include <iostream>

#include "roulette/beamlet.h"

using namespace roulette;

TEST(BeamletTest, generates_good_directional_vectors) {
  RandomGenerator generator;

  Beamlet beamlet(
    ThreeVector(0, 0, 0),
    ThreeVector(-1, -1, 10),
    ThreeVector(1, -1, 10),
    ThreeVector(1, 1, 10)
  );

  for (int i = 0; i < 10; ++i) {
    ThreeVector u = beamlet.directional_unit_vector(generator);
    ASSERT_FLOAT_EQ(u.magnitude2(), 1);

    double t = 10 / u(2);

    ThreeVector p = beamlet.source() + t * u;

    ASSERT_GE(p(0), -1);
    ASSERT_LE(p(0), 1);
    ASSERT_GE(p(1), -1);
    ASSERT_LE(p(1), 1);
    ASSERT_EQ(p(2), 10);
  }
}
