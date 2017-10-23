#pragma once

#include "../test_helper.h"

#include "roulette/json.h"
#include "roulette/sources/bifocal_source.h"

using namespace roulette;

TEST(BifocalSourceTest, test_aperture_generator) {
  RandomGenerator generator;
  sources::BifocalSource source(Json::json_document_from_file_or_string(std::string(
    "{ \
      \"origin_generator\": { \
        \"geometry\": { \
          \"type\": \"Point\", \
          \"point\": [2, 4, 8] \
        }, \
        \"spectrum\": { \
          \"type\": \"pdf\", \
          \"data\": [[5.0, 0], [5.5, 0.5], [6.0, 1]] \
        } \
      }, \
      \"aperture_generator\": { \
        \"geometry\": { \
          \"type\": \"Point\", \
          \"point\": [3, 4, 7] \
        } \
      } \
    }"
  )));

  auto particle = source.particle(generator);

  EXPECT_EQ(particle->position()(0), 2);
  EXPECT_EQ(particle->position()(1), 4);
  EXPECT_EQ(particle->position()(2), 8);

  EXPECT_NEAR(particle->momentum().direction_unit_vector()(0), 1 / std::sqrt(2), 0.0000001);
  EXPECT_EQ(particle->momentum().direction_unit_vector()(1), 0);
  EXPECT_NEAR(particle->momentum().direction_unit_vector()(2), -1 / std::sqrt(2), 0.0000001);
}

TEST(BifocalSourceTest, test_direction_generator) {
  RandomGenerator generator;
  sources::BifocalSource source(Json::json_document_from_file_or_string(std::string(
    "{ \
      \"origin_generator\": { \
        \"geometry\": { \
          \"type\": \"Point\", \
          \"point\": [2, 4, 8] \
        }, \
        \"spectrum\": { \
          \"type\": \"pdf\", \
          \"data\": [[5.0, 0], [5.5, 0.5], [6.0, 1]] \
        } \
      }, \
      \"direction_generator\": { \
        \"geometry\": { \
          \"type\": \"Point\", \
          \"point\": [-1,1,0] \
        } \
      } \
    }"
  )));

  auto particle = source.particle(generator);

  EXPECT_EQ(particle->position()(0), 2);
  EXPECT_EQ(particle->position()(1), 4);
  EXPECT_EQ(particle->position()(2), 8);

  EXPECT_NEAR(particle->momentum().direction_unit_vector()(0), -1 / std::sqrt(2), 0.0000001);
  EXPECT_NEAR(particle->momentum().direction_unit_vector()(1), 1 / std::sqrt(2), 0.0000001);
  EXPECT_EQ(particle->momentum().direction_unit_vector()(2), 0);
}

/*   EXPECT_THROW( */
/*     auto point = geometries::GeometryFactory::geometry(Json::json_document_from_file_or_string(std::string( */
/*       "{\"type\":\"DoesNotExist\"}" */
/*     ))), */
/*     geometries::InvalidGeometry */
/*   ); */

/* TEST(GeometryFactoryTest, geometry_factory_point_test) { */
/*   RandomGenerator generator; */
/*   auto point = geometries::GeometryFactory::geometry(Json::json_document_from_file_or_string(std::string( */
/*     "{\"type\":\"Point\",\"point\":[1, 2, 5]}" */
/*   ))); */

/*   auto v = point->sample(generator); */
/*   EXPECT_EQ(v(0), 1); */
/*   EXPECT_EQ(v(1), 2); */
/*   EXPECT_EQ(v(2), 5); */

/*   EXPECT_THROW( */
/*     auto point = geometries::GeometryFactory::geometry(Json::json_document_from_file_or_string(std::string( */
/*       "{\"type\":\"Point\"}" */
/*     ))), */
/*     geometries::InvalidGeometry */
/*   ); */
/* } */

/* TEST(GeometryFactoryTest, geometry_factory_rectangle_test) { */
/*   RandomGenerator generator; */
/*   auto rectangle = geometries::GeometryFactory::geometry(Json::json_document_from_file_or_string(std::string( */
/*     "{\"type\":\"Rectangle\",\"bottom_left\":[1,0,3],\"bottom_right\":[2,0,3],\"top_right\":[2,0,4]}" */
/*   ))); */

/*   auto v = rectangle->sample(generator); */
/*   EXPECT_GE(v(0), 1); */
/*   EXPECT_LT(v(0), 2); */
/*   EXPECT_EQ(v(1), 0); */
/*   EXPECT_GE(v(2), 3); */
/*   EXPECT_LT(v(2), 4); */

/*   EXPECT_THROW( */
/*     auto rectangle = geometries::GeometryFactory::geometry(Json::json_document_from_file_or_string(std::string( */
/*       "{\"type\":\"Rectangle\",\"bottom_right\":[2,0,3],\"top_right\":[2,0,4]}" */
/*     ))), */
/*     geometries::InvalidGeometry */
/*   ); */

/*   EXPECT_THROW( */
/*     auto rectangle = geometries::GeometryFactory::geometry(Json::json_document_from_file_or_string(std::string( */
/*       "{\"type\":\"Rectangle\",\"bottom_left\":[2,0,3],\"top_right\":[2,0,4]}" */
/*     ))), */
/*     geometries::InvalidGeometry */
/*   ); */

/*   EXPECT_THROW( */
/*     auto rectangle = geometries::GeometryFactory::geometry(Json::json_document_from_file_or_string(std::string( */
/*       "{\"type\":\"Rectangle\",\"bottom_left\":[2,0,3],\"bottom_right\":[2,0,4]}" */
/*     ))), */
/*     geometries::InvalidGeometry */
/*   ); */
/* } */

/* TEST(GeometryFactoryTest, geometry_factory_ellipse_test) { */
/*   RandomGenerator generator; */
/*   auto ellipse = geometries::GeometryFactory::geometry(Json::json_document_from_file_or_string(std::string( */
/*     "{\"type\":\"Ellipse\",\"center\":[0,0,5],\"vertex\":[1,0,5],\"covertex\":[0,2,5]}" */
/*   ))); */

/*   auto v = ellipse->sample(generator); */
/*   EXPECT_LT(v(0)*v(0) + v(1)*v(1)/2/2, 1); */
/*   EXPECT_EQ(v(2), 5); */

/*   EXPECT_THROW( */
/*     auto ellipse = geometries::GeometryFactory::geometry(Json::json_document_from_file_or_string(std::string( */
/*       "{\"type\":\"Ellipse\",\"center\":[2,0,3],\"vertex\":[2,0,4]}" */
/*     ))), */
/*     geometries::InvalidGeometry */
/*   ); */

/*   EXPECT_THROW( */
/*     auto ellipse = geometries::GeometryFactory::geometry(Json::json_document_from_file_or_string(std::string( */
/*       "{\"type\":\"Ellipse\",\"center\":[2,0,3],\"covertex\":[2,0,4]}" */
/*     ))), */
/*     geometries::InvalidGeometry */
/*   ); */

/*   EXPECT_THROW( */
/*     auto ellipse = geometries::GeometryFactory::geometry(Json::json_document_from_file_or_string(std::string( */
/*       "{\"type\":\"Ellipse\",\"vertex\":[2,0,3],\"covertex\":[2,0,4]}" */
/*     ))), */
/*     geometries::InvalidGeometry */
/*   ); */
/* } */

/* TEST(GeometryFactoryTest, geometry_factory_spherical_shell_test) { */
/*   RandomGenerator generator; */
/*   auto shell = geometries::GeometryFactory::geometry(Json::json_document_from_file_or_string(std::string( */
/*     "{\"type\":\"SphericalShell\",\"center\":[2,1,-1],\"radius\":1.5}" */
/*   ))); */

/*   auto v = shell->sample(generator); */
/*   EXPECT_NEAR((v - ThreeVector(2, 1, -1)).magnitude(), 1.5, 0.0005); */

/*   EXPECT_THROW( */
/*     auto shell = geometries::GeometryFactory::geometry(Json::json_document_from_file_or_string(std::string( */
/*       "{\"type\":\"SphericalShell\",\"radius\":1.5}" */
/*     ))), */
/*     geometries::InvalidGeometry */
/*   ); */

/*   EXPECT_THROW( */
/*     auto shell = geometries::GeometryFactory::geometry(Json::json_document_from_file_or_string(std::string( */
/*       "{\"type\":\"SphericalShell\",\"center\":[1,2,3]}" */
/*     ))), */
/*     geometries::InvalidGeometry */
/*   ); */
/* } */

/* TEST(GeometryFactoryTest, geometry_factory_hemispherical_shell_test) { */
/*   RandomGenerator generator; */
/*   auto shell = geometries::GeometryFactory::geometry(Json::json_document_from_file_or_string(std::string( */
/*     "{\"type\":\"HemisphericalShell\",\"center\":[1,1,1],\"apex\":[4,1,1]}" */
/*   ))); */

/*   auto v = shell->sample(generator); */
/*   EXPECT_NEAR((v - ThreeVector(1,1,1)).magnitude(), 3, 0.0005); */
/*   EXPECT_GT((v - ThreeVector(1,1,1))(0), 0); */

/*   EXPECT_THROW( */
/*     auto shell = geometries::GeometryFactory::geometry(Json::json_document_from_file_or_string(std::string( */
/*       "{\"type\":\"HemiphericalShell\",\"center\":[1,2,3]}" */
/*     ))), */
/*     geometries::InvalidGeometry */
/*   ); */

/*   EXPECT_THROW( */
/*     auto shell = geometries::GeometryFactory::geometry(Json::json_document_from_file_or_string(std::string( */
/*       "{\"type\":\"HemiphericalShell\",\"apex\":[1,2,3]}" */
/*     ))), */
/*     geometries::InvalidGeometry */
/*   ); */
/* } */
