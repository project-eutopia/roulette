#include "roulette/geometries/rectangle.h"

namespace roulette {
  namespace geometries {
    Rectangle::Rectangle(const ThreeVector& p0, const ThreeVector& p1, const ThreeVector& p2)
      :
        m_p0(p0),
        m_u1(p1 - p0),
        m_u2(p2 - p1),
        m_u_pdf(std::make_shared<const Pdf>(0, 1)),
        m_v_pdf(std::make_shared<const Pdf>(0, 1))
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

      auto rectangle = std::make_shared<Rectangle>(
        ThreeVector(data["bottom_left"]),
        ThreeVector(data["bottom_right"]),
        ThreeVector(data["top_right"])
      );

      if (data.HasMember("u_pdf")) {
        rectangle->m_u_pdf = std::make_shared<const Pdf>(data["u_pdf"]);
        if (rectangle->m_u_pdf->min() != 0) { throw std::runtime_error("PDF must start at 0"); }
        if (rectangle->m_u_pdf->max() != 1) { throw std::runtime_error("PDF must end at 1"); }
      }
      else {
        rectangle->m_u_pdf = std::make_shared<const Pdf>(0, 1);
      }

      if (data.HasMember("v_pdf")) {
        rectangle->m_v_pdf = std::make_shared<const Pdf>(data["v_pdf"]);
        if (rectangle->m_v_pdf->min() != 0) { throw std::runtime_error("PDF must start at 0"); }
        if (rectangle->m_v_pdf->max() != 1) { throw std::runtime_error("PDF must end at 1"); }
      }
      else {
        rectangle->m_v_pdf = std::make_shared<const Pdf>(0, 1);
      }

      return rectangle;
    }

    ThreeVector Rectangle::sample(RandomGenerator& generator) const {
      return m_p0 + (*m_u_pdf)(generator) * m_u1 + (*m_v_pdf)(generator) * m_u2;
    }
  }
}
