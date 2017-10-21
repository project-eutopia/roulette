#pragma once

#include "../test_helper.h"

#include "roulette/geometries/spherical_shell.h"

using namespace roulette;

TEST(GeometrySphericalShellTest, spherical_shell_test) {
  double radius = 2;
  ThreeVector center{1, 2, 5};
  RandomGenerator generator;
  geometries::SphericalShell shell(center, radius);

  for (int i = 0; i < 20; ++i) {
    auto three_vector = shell.sample(generator);
    EXPECT_NEAR((three_vector - center).magnitude(), radius, 0.0001);
  }
}

TEST(GeometrySphericalShellTest, spherical_shell_error_test) {
  // Verify exceptions thrown
  try {
    geometries::SphericalShell shell(
      {0, 0, 0},
      0
    );
    FAIL();
  }
  catch( const geometries::InvalidGeometry& err ) {
    // check exception
    ASSERT_STREQ("SphericalShell must have positive radius", err.what());
  }
}
