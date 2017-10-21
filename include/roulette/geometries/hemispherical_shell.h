#pragma once

#include <memory>
#include "roulette/geometries/geometry.h"
#include "roulette/rotation_matrix.h"

#include "rapidjson/document.h"

namespace roulette {
  namespace geometries {
    class HemisphericalShell : public Geometry {
      private:
        const ThreeVector m_center;
        // Either pointing up or down, depending on which hemisphere the apex is in
        const ThreeVector m_zaxis;
        // Rotates points from m_zaxis direction into correct apex direction
        const RotationMatrix m_rotation;

      public:
        static std::shared_ptr<HemisphericalShell> from_json(const rapidjson::Value& data);
        HemisphericalShell(const ThreeVector& center, const ThreeVector& apex);

        ThreeVector sample(RandomGenerator& generator) const;
    };
  };
};
