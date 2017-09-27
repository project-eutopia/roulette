#pragma once

#include "test_helper.h"

#include <cmath>

#include "roulette/distributions/uniform.h"
#include "roulette/affine_transformation.h"

using namespace roulette;

TEST(AffineTransformationTest, test_identity) {
  RandomGenerator g;
  distributions::Uniform uniform(-5, 5);

  const double eps = 1e-12;

  const ThreeVector vec(uniform(g), uniform(g), uniform(g));
  const ThreeVector point(uniform(g), uniform(g), uniform(g));

  const AffineTransformation identity{};

  EXPECT_NEAR((point - identity.transform_point(point)).magnitude(), 0, eps);
  EXPECT_NEAR((vec - identity.transform_vector(vec)).magnitude(), 0, eps);
}

TEST(AffineTransformationTest, test_rotation) {
  RandomGenerator g;
  distributions::Uniform uniform(-5, 5);

  const double eps = 1e-12;

  ThreeVector tmp;
  tmp = ThreeVector(uniform(g), uniform(g), uniform(g));
  const ThreeVector u = tmp.direction_unit_vector();

  tmp = ThreeVector(uniform(g), uniform(g), uniform(g));
  const ThreeVector v = tmp.direction_unit_vector();

  const AffineTransformation rotation(RotationMatrix::rotationUtoV(u, v));
  EXPECT_NEAR((v - rotation.transform_point(u)).magnitude(), 0, eps);
  EXPECT_NEAR((v - rotation.transform_vector(u)).magnitude(), 0, eps);

  const AffineTransformation inverse = rotation.inv();
  EXPECT_NEAR((u - inverse.transform_point(v)).magnitude(), 0, eps);
  EXPECT_NEAR((u - inverse.transform_vector(v)).magnitude(), 0, eps);

  const AffineTransformation identity = inverse * rotation;
  EXPECT_NEAR((u - identity.transform_point(u)).magnitude(), 0, eps);
  EXPECT_NEAR((u - identity.transform_vector(u)).magnitude(), 0, eps);
}

TEST(AffineTransformationTest, test_translation) {
  RandomGenerator g;
  distributions::Uniform uniform(-5, 5);

  const double eps = 1e-12;

  const ThreeVector zero;

  ThreeVector tmp;
  tmp = ThreeVector(uniform(g), uniform(g), uniform(g));
  const ThreeVector u = tmp.direction_unit_vector();

  tmp = ThreeVector(uniform(g), uniform(g), uniform(g));
  const ThreeVector v = tmp.direction_unit_vector();

  const AffineTransformation translation(u);
  EXPECT_NEAR((u - translation.transform_point(zero)).magnitude(), 0, eps);
  EXPECT_NEAR((zero - translation.transform_vector(zero)).magnitude(), 0, eps);
  EXPECT_NEAR((u+v - translation.transform_point(v)).magnitude(), 0, eps);
  EXPECT_NEAR((v - translation.transform_vector(v)).magnitude(), 0, eps);

  const AffineTransformation inverse = translation.inv();
  EXPECT_NEAR((-u - inverse.transform_point(zero)).magnitude(), 0, eps);
  EXPECT_NEAR((zero - inverse.transform_vector(zero)).magnitude(), 0, eps);
  EXPECT_NEAR((v-u - inverse.transform_point(v)).magnitude(), 0, eps);
  EXPECT_NEAR((v - inverse.transform_vector(v)).magnitude(), 0, eps);

  const AffineTransformation identity = inverse * translation;
  EXPECT_NEAR((zero - identity.transform_point(zero)).magnitude(), 0, eps);
  EXPECT_NEAR((zero - identity.transform_vector(zero)).magnitude(), 0, eps);
  EXPECT_NEAR((v - identity.transform_point(v)).magnitude(), 0, eps);
  EXPECT_NEAR((v - identity.transform_vector(v)).magnitude(), 0, eps);
}

TEST(AffineTransformationTest, test_general_transformation) {
  RandomGenerator g;
  distributions::Uniform uniform(-5, 5);

  const double eps = 1e-12;

  const ThreeVector zero;

  ThreeVector tmp;
  tmp = ThreeVector(uniform(g), uniform(g), uniform(g));
  const ThreeVector u = tmp.direction_unit_vector();

  tmp = ThreeVector(uniform(g), uniform(g), uniform(g));
  const ThreeVector v = tmp.direction_unit_vector();

  tmp = ThreeVector(uniform(g), uniform(g), uniform(g));
  const ThreeVector w = tmp.direction_unit_vector();
  const RotationMatrix rotation = RotationMatrix::rotationUtoV(u, v);

  const AffineTransformation transformation(rotation, w);
  tmp = ThreeVector(uniform(g), uniform(g), uniform(g));
  EXPECT_NEAR((rotation*tmp + w - transformation.transform_point(tmp)).magnitude(), 0, eps);
  EXPECT_NEAR((rotation*tmp - transformation.transform_vector(tmp)).magnitude(), 0, eps);

  const AffineTransformation inverse = transformation.inv();
  tmp = ThreeVector(uniform(g), uniform(g), uniform(g));
  EXPECT_NEAR((rotation.inv()*tmp - rotation.inv()*w - inverse.transform_point(tmp)).magnitude(), 0, eps);
  EXPECT_NEAR((rotation.inv()*tmp - inverse.transform_vector(tmp)).magnitude(), 0, eps);

  const AffineTransformation identity = inverse * transformation;
  tmp = ThreeVector(uniform(g), uniform(g), uniform(g));
  EXPECT_NEAR((tmp - identity.transform_point(tmp)).magnitude(), 0, eps);
  EXPECT_NEAR((tmp - identity.transform_vector(tmp)).magnitude(), 0, eps);
}
