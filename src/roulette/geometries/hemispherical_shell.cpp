#include "roulette/geometries/hemispherical_shell.h"
#include "roulette/math.h"

namespace roulette {
  namespace geometries {
    HemisphericalShell::HemisphericalShell(const ThreeVector& center, const ThreeVector& apex)
      :
        m_center(center),
        m_zaxis((apex - center)(2) >= 0 ? ThreeVector(0,0,(apex-center).magnitude()) : ThreeVector(0,0,-(apex-center).magnitude())),
        m_rotation(RotationMatrix::rotationUtoV(m_zaxis.direction_unit_vector(), (apex - center).direction_unit_vector()))
    {
    }

    std::shared_ptr<HemisphericalShell> HemisphericalShell::from_json(const rapidjson::Value& data)
    {
      if (!data.HasMember("center")) throw InvalidGeometry("HemisphericalShell requires \"center\"");
      if (!data.HasMember("apex")) throw InvalidGeometry("HemisphericalShell requires \"apex\"");

      return std::make_shared<HemisphericalShell>(
        ThreeVector(data["center"]),
        ThreeVector(data["apex"])
      );
    }

    ThreeVector HemisphericalShell::sample(RandomGenerator& generator) const {
      double height = m_zaxis(2)*generator.uniform();
      double rho = std::sqrt(m_zaxis(2)*m_zaxis(2) - height*height);
      double phi = 2*M_PI*generator.uniform();

      return m_center + m_rotation * ThreeVector(rho*math::cos(phi), rho*math::sin(phi), height);
    }
  }
}
