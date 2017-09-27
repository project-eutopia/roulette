#pragma once

#include "test_helper.h"

#include <iostream>

#include <cmath>
#include "roulette/rotation_matrix.h"
#include "roulette/three_vector.h"

using namespace roulette;

TEST(RotationMatrixTest, test_identity) {
  const ThreeVector xhat(1, 0, 0);
  const ThreeVector yhat(0, 1, 0);
  const ThreeVector zhat(0, 0, 1);

  const double eps = 0.0000001;

  const RotationMatrix identity;

  EXPECT_NEAR((identity*xhat)(0), 1, eps);
  EXPECT_NEAR((identity*xhat)(1), 0, eps);
  EXPECT_NEAR((identity*xhat)(2), 0, eps);

  EXPECT_NEAR((identity*yhat)(0), 0, eps);
  EXPECT_NEAR((identity*yhat)(1), 1, eps);
  EXPECT_NEAR((identity*yhat)(2), 0, eps);

  EXPECT_NEAR((identity*zhat)(0), 0, eps);
  EXPECT_NEAR((identity*zhat)(1), 0, eps);
  EXPECT_NEAR((identity*zhat)(2), 1, eps);
}

TEST(RotationMatrixTest, test_simple_rotations) {
  const ThreeVector xhat(1, 0, 0);
  const ThreeVector yhat(0, 1, 0);
  const ThreeVector zhat(0, 0, 1);

  const double eps = 0.0000001;
  const double sqrt3 = std::sqrt(3.0);

  const RotationMatrix rotx = RotationMatrix::rotationX(M_PI/6);
  const RotationMatrix roty = RotationMatrix::rotationY(M_PI/6);
  const RotationMatrix rotz = RotationMatrix::rotationZ(M_PI/6);

  {
    const ThreeVector res = rotx*xhat;
    EXPECT_NEAR(res(0), 1, eps);
    EXPECT_NEAR(res(1), 0, eps);
    EXPECT_NEAR(res(2), 0, eps);
  }

  {
    const ThreeVector res = rotx*yhat;
    EXPECT_NEAR(res(0), 0, eps);
    EXPECT_NEAR(res(1), sqrt3/2, eps);
    EXPECT_NEAR(res(2), 0.5, eps);
  }

  {
    const ThreeVector res = rotx*zhat;
    EXPECT_NEAR(res(0), 0, eps);
    EXPECT_NEAR(res(1), -0.5, eps);
    EXPECT_NEAR(res(2), sqrt3/2, eps);
  }

  {
    const ThreeVector res = roty*xhat;
    EXPECT_NEAR(res(0), sqrt3/2, eps);
    EXPECT_NEAR(res(1), 0, eps);
    EXPECT_NEAR(res(2), -0.5, eps);
  }

  {
    const ThreeVector res = roty*yhat;
    EXPECT_NEAR(res(0), 0, eps);
    EXPECT_NEAR(res(1), 1, eps);
    EXPECT_NEAR(res(2), 0, eps);
  }

  {
    const ThreeVector res = roty*zhat;
    EXPECT_NEAR(res(0), 0.5, eps);
    EXPECT_NEAR(res(1), 0, eps);
    EXPECT_NEAR(res(2), sqrt3/2, eps);
  }

  {
    const ThreeVector res = rotz*xhat;
    EXPECT_NEAR(res(0), sqrt3/2, eps);
    EXPECT_NEAR(res(1), 0.5, eps);
    EXPECT_NEAR(res(2), 0, eps);
  }

  {
    const ThreeVector res = rotz*yhat;
    EXPECT_NEAR(res(0), -0.5, eps);
    EXPECT_NEAR(res(1), sqrt3/2, eps);
    EXPECT_NEAR(res(2), 0, eps);
  }

  {
    const ThreeVector res = rotz*zhat;
    EXPECT_NEAR(res(0), 0, eps);
    EXPECT_NEAR(res(1), 0, eps);
    EXPECT_NEAR(res(2), 1, eps);
  }
}

TEST(RotationMatrixTest, test_u_v_rotations) {
  const ThreeVector xhat(1, 0, 0);
  const ThreeVector yhat(0, 1, 0);
  const ThreeVector zhat(0, 0, 1);

  const double eps = 0.0000001;

  ThreeVector tmp;
  tmp = ThreeVector(0.2528, -3.1, 1.334);
  const ThreeVector u = tmp.direction_unit_vector();

  tmp = ThreeVector(1.8453, 0.054, -2.004);
  const ThreeVector v = tmp.direction_unit_vector();

  const RotationMatrix rotation = RotationMatrix::rotationUtoV(u, v);

  const ThreeVector rotated = rotation * u;

  EXPECT_NEAR(rotated(0), v(0), eps);
  EXPECT_NEAR(rotated(1), v(1), eps);
  EXPECT_NEAR(rotated(2), v(2), eps);

  const RotationMatrix inv = rotation.inv();

  const ThreeVector inv_rotated = inv * v;

  EXPECT_NEAR(inv_rotated(0), u(0), eps);
  EXPECT_NEAR(inv_rotated(1), u(1), eps);
  EXPECT_NEAR(inv_rotated(2), u(2), eps);

  {
    const RotationMatrix identity = inv * rotation;

    EXPECT_NEAR((identity*u)(0), u(0), eps);
    EXPECT_NEAR((identity*u)(1), u(1), eps);
    EXPECT_NEAR((identity*u)(2), u(2), eps);
    EXPECT_NEAR((identity*v)(0), v(0), eps);
    EXPECT_NEAR((identity*v)(1), v(1), eps);
    EXPECT_NEAR((identity*v)(2), v(2), eps);
  }

  {
    const RotationMatrix identity = rotation * inv;

    EXPECT_NEAR((identity*u)(0), u(0), eps);
    EXPECT_NEAR((identity*u)(1), u(1), eps);
    EXPECT_NEAR((identity*u)(2), u(2), eps);
    EXPECT_NEAR((identity*v)(0), v(0), eps);
    EXPECT_NEAR((identity*v)(1), v(1), eps);
    EXPECT_NEAR((identity*v)(2), v(2), eps);
  }
}
