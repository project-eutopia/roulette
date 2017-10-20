#include "roulette/geometries/point.h"

namespace roulette {
  namespace geometries {
    std::shared_ptr<Point> Point::from_json(const rapidjson::Value& data) {
      if (!data.HasMember("point")) throw InvalidGeometry("Point requires \"point\"");
      return std::make_shared<Point>(ThreeVector(data["point"]));
    }

    Point::Point(const ThreeVector& three_vector)
      : m_point(three_vector)
    {}

    ThreeVector Point::sample(RandomGenerator& generator) const { return m_point; }
  }
}
