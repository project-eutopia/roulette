#include "roulette/geometries/spherical_shell.h"
#include "roulette/math.h"

namespace roulette {
  namespace geometries {
    SphericalShell::SphericalShell(const ThreeVector& center, double radius)
      :
        m_center(center),
        m_radius(radius)
    {
      // Check non-degenerate
      // NOTE: We allow rhombus shapes, so u1 and u2 are allowed to not be perpendicular
      if (m_radius <= 0) {
        throw InvalidGeometry("SphericalShell must have positive radius");
      }
    }

    std::shared_ptr<SphericalShell> SphericalShell::from_json(const rapidjson::Value& data)
    {
      if (!data.HasMember("center")) throw InvalidGeometry("SphericalShell requires \"center\"");
      if (!data.HasMember("radius") || !data["radius"].IsNumber()) throw InvalidGeometry("SphericalShell requires \"radius\"");

      return std::make_shared<SphericalShell>(
        ThreeVector(data["center"]),
        data["radius"].GetDouble()
      );
    }

    ThreeVector SphericalShell::sample(RandomGenerator& generator) const {
      double height = m_radius * (2*generator.uniform() - 1);
      double rho = std::sqrt(m_radius*m_radius - height*height);
      double phi = 2*M_PI*generator.uniform();

      return ThreeVector(
        m_center(0) + rho*math::cos(phi),
        m_center(1) + rho*math::sin(phi),
        m_center(2) + height
      );
    }
  }
}
