#include "roulette/geometries/rectangle.h"

namespace roulette {
  namespace geometries {
    Rectangle::Rectangle(const ThreeVector& p0, const ThreeVector& p1, const ThreeVector& p2)
      :
        m_p0(p0),
        m_u1(p1 - p0),
        m_u2(p2 - p1)
    {
      // Check non-degenerate
      // NOTE: We allow rhombus shapes, so u1 and u2 are allowed to not be perpendicular
      if (m_u1.cross(m_u2).magnitude() == 0) {
        throw InvalidGeometry("Rectangle must be non-degenerate");
      }
    }

    std::shared_ptr<Rectangle> Rectangle::from_json(const rapidjson::Value& data)
    {
      if (!data.HasMember("bottom_left")) throw InvalidGeometry("Rectangle requires \"bottom_left\"");
      if (!data.HasMember("bottom_right")) throw InvalidGeometry("Rectangle requires \"bottom_right\"");
      if (!data.HasMember("top_right")) throw InvalidGeometry("Rectangle requires \"top_right\"");

      return std::make_shared<Rectangle>(
        ThreeVector(data["bottom_left"]),
        ThreeVector(data["bottom_right"]),
        ThreeVector(data["top_right"])
      );
    }

    ThreeVector Rectangle::sample(RandomGenerator& generator) const {
      return m_p0 + generator.uniform() * m_u1 + generator.uniform() * m_u2;
    }
  }
}
