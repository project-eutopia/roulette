#include "roulette/geometries/geometry_factory.h"

#include "roulette/geometries/point.h"
#include "roulette/geometries/rectangle.h"
#include "roulette/geometries/ellipse.h"
#include "roulette/geometries/spherical_shell.h"
#include "roulette/geometries/hemispherical_shell.h"

namespace roulette {
  namespace geometries {
    std::shared_ptr<Geometry> GeometryFactory::geometry(const rapidjson::Value& data) {
      assert(data.HasMember("type"));

      if (data["type"].GetString() == std::string("Point")) {
        return Point::from_json(data);
      }
      else if (data["type"].GetString() == std::string("Rectangle")) {
        return Rectangle::from_json(data);
      }
      else if (data["type"].GetString() == std::string("Ellipse")) {
        return Ellipse::from_json(data);
      }
      else if (data["type"].GetString() == std::string("SphericalShell")) {
        return SphericalShell::from_json(data);
      }
      else if (data["type"].GetString() == std::string("HemisphericalShell")) {
        return HemisphericalShell::from_json(data);
      }
      else {
        // Unhandled source type
        throw InvalidGeometry("Unknown geometry type");
      }
    }
  };
};
