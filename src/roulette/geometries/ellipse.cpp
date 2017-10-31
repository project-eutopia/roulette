#include "roulette/geometries/ellipse.h"
#include "roulette/math.h"

namespace roulette {
  namespace geometries {
    Ellipse::Ellipse(const ThreeVector& center, const ThreeVector& vertex, const ThreeVector& covertex)
      :
        m_center(center),
        m_u1((vertex - center).direction_unit_vector()),
        m_u2((covertex - center).direction_unit_vector()),
        m_a((vertex - center).magnitude()),
        m_b((covertex - center).magnitude()),
        m_radius_pdf(std::make_shared<const Pdf>(0, 1))
    {
      // Check non-degenerate
      if (m_u1.cross(m_u2).magnitude() == 0) {
        throw InvalidGeometry("Ellipse must be non-degenerate");
      }

      if (std::abs(m_u1.dot(m_u2)) > 0.000001*m_u1.magnitude()) {
        throw InvalidGeometry("Ellipse vertex and covertex must be perpendicular");
      }
    }

    std::shared_ptr<Ellipse> Ellipse::from_json(const rapidjson::Value& data)
    {
      if (!data.HasMember("center")) throw InvalidGeometry("Ellipse requires \"center\"");
      if (!data.HasMember("vertex")) throw InvalidGeometry("Ellipse requires \"vertex\"");
      if (!data.HasMember("covertex")) throw InvalidGeometry("Ellipse requires \"covertex\"");

      auto ellipse = std::make_shared<Ellipse>(
        ThreeVector(data["center"]),
        ThreeVector(data["vertex"]),
        ThreeVector(data["covertex"])
      );

      if (data.HasMember("radius_pdf")) {
        ellipse->m_radius_pdf = std::make_shared<const Pdf>(data["radius_pdf"]);
        if (ellipse->m_radius_pdf->min() != 0) { throw std::runtime_error("PDF must start at 0"); }
        if (ellipse->m_radius_pdf->max() != 1) { throw std::runtime_error("PDF must end at 1"); }
      }
      else {
        ellipse->m_radius_pdf = std::make_shared<const Pdf>(0, 1);
      }

      return ellipse;
    }

    ThreeVector Ellipse::sample(RandomGenerator& generator) const {
      double theta = this->random_theta(generator);
      double radius = this->radius(theta) * std::sqrt((*m_radius_pdf)(generator));
      return m_center + radius*math::cos(theta)*m_u1 + radius*math::sin(theta)*m_u2;
    }

    double Ellipse::random_theta(RandomGenerator& generator) const {
      // Work in first sector only
      double u = generator.uniform() / 4.0;
      double theta = std::atan(m_b / m_a * std::tan(2*M_PI*u));

      // Then pick a random sector and shift there
      switch ((int)(4*generator.uniform())) {
        case 0:
          return theta;
        case 1:
          return M_PI - theta;
        case 2:
          return M_PI + theta;
        case 3:
          return 2*M_PI - theta;
        default:
          return 0;
      }
    };

    double Ellipse::radius(double theta) const {
      double c = math::cos(theta);
      double s = math::sin(theta);
      return m_a * m_b / std::sqrt(m_b*m_b*c*c + m_a*m_a*s*s);
    };
  }
}
