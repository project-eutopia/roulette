#pragma once

#include "../test_helper.h"

#include "roulette/geometries/ellipse.h"

using namespace roulette;

TEST(GeometryEllipseTest, ellipse_test) {
  RandomGenerator generator;
  geometries::Ellipse ellipse(
    {0, 0, 0},
    {1, 2, 0},
    {0, 0, 1}
  );

  double a = ThreeVector(1,2,0).magnitude();
  double b = 1.0;
  ThreeVector u1 = ThreeVector(1,2,0).direction_unit_vector();
  ThreeVector u2 = ThreeVector(0,0,1).direction_unit_vector();

  for (int i = 0; i < 20; ++i) {
    auto three_vector = ellipse.sample(generator);

    EXPECT_EQ(2*three_vector(0), three_vector(1));

    double x = three_vector.dot(u1);
    double y = three_vector.dot(u2);

    EXPECT_LE(x*x/a/a + y*y/b/b, 1);
  }
}

TEST(GeometryEllipseTest, ellipse_error_test) {
  // Verify exceptions thrown
  try {
    geometries::Ellipse ellispse(
      {0, 0, 0},
      {1, 0, 0},
      {-1, 0, 0}
    );
    FAIL();
  }
  catch( const geometries::InvalidGeometry& err ) {
    // check exception
    ASSERT_STREQ("Ellipse must be non-degenerate", err.what());
  }

  try {
    geometries::Ellipse ellipse(
      {0, 0, 0},
      {1, 0, 0},
      {0, 0, 0}
    );
    FAIL();
  }
  catch( const geometries::InvalidGeometry& err ) {
    // check exception
    ASSERT_STREQ("Ellipse must be non-degenerate", err.what());
  }

  try {
    geometries::Ellipse ellipse(
      {0, 0, 0},
      {0, 0, 0},
      {1, 0, 0}
    );
    FAIL();
  }
  catch( const geometries::InvalidGeometry& err ) {
    // check exception
    ASSERT_STREQ("Ellipse must be non-degenerate", err.what());
  }

  try {
    geometries::Ellipse ellipse(
      {0, 0, 0},
      {1, 0, 0},
      {1, 1, 0}
    );
    FAIL();
  }
  catch( const geometries::InvalidGeometry& err ) {
    // check exception
    ASSERT_STREQ("Ellipse vertex and covertex must be perpendicular", err.what());
  }
}
