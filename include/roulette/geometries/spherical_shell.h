#pragma once

#include <memory>
#include "roulette/geometries/geometry.h"

#include "rapidjson/document.h"

namespace roulette {
  namespace geometries {
    class SphericalShell : public Geometry {
      private:
        const ThreeVector m_center;
        double m_radius;

      public:
        static std::shared_ptr<SphericalShell> from_json(const rapidjson::Value& data);
        SphericalShell(const ThreeVector& center, double radius);

        ThreeVector sample(RandomGenerator& generator) const;
    };
  };
};
