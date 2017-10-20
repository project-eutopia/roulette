#pragma once

#include "../test_helper.h"

#include "roulette/json.h"
#include "roulette/geometries/geometry_factory.h"

using namespace roulette;

TEST(GeometryFactoryTest, geometry_factory_point_test) {
  RandomGenerator generator;
  auto point = geometries::GeometryFactory::geometry(Json::json_document_from_file_or_string(std::string(
    "{\"type\":\"Point\",\"point\":[1, 2, 5]}"
  )));

  auto v = point->sample(generator);
  EXPECT_EQ(v(0), 1);
  EXPECT_EQ(v(1), 2);
  EXPECT_EQ(v(2), 5);
}

TEST(GeometryFactoryTest, geometry_factory_rectangle_test) {
  RandomGenerator generator;
  auto rectangle = geometries::GeometryFactory::geometry(Json::json_document_from_file_or_string(std::string(
    "{\"type\":\"Rectangle\",\"bottom_left\":[1,0,3],\"bottom_right\":[2,0,3],\"top_right\":[2,0,4]}"
  )));

  auto v = rectangle->sample(generator);
  EXPECT_GE(v(0), 1);
  EXPECT_LT(v(0), 2);
  EXPECT_EQ(v(1), 0);
  EXPECT_GE(v(2), 3);
  EXPECT_LT(v(2), 4);
}

TEST(GeometryFactoryTest, geometry_factory_ellipse_test) {
  RandomGenerator generator;
  auto ellipse = geometries::GeometryFactory::geometry(Json::json_document_from_file_or_string(std::string(
    "{\"type\":\"Ellipse\",\"center\":[0,0,5],\"vertex\":[1,0,5],\"covertex\":[0,2,5]}"
  )));

  auto v = ellipse->sample(generator);
  EXPECT_LT(v(0)*v(0) + v(1)*v(1)/2/2, 1);
  EXPECT_EQ(v(2), 5);
}
