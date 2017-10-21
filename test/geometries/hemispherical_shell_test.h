#pragma once

#include "../test_helper.h"

#include "roulette/geometries/hemispherical_shell.h"

using namespace roulette;

TEST(GeometryHemisphericalShellTest, hemispherical_shell_test) {
  ThreeVector center{1, 2, 5};
  ThreeVector apex{1, 2, 3};
  RandomGenerator generator;
  geometries::HemisphericalShell shell(center, apex);

  for (int i = 0; i < 20; ++i) {
    auto three_vector = shell.sample(generator);
    EXPECT_NEAR((three_vector - center).magnitude(), 2, 0.0001);
    EXPECT_GT((three_vector - center).dot(apex-center), 0);
  }
}
