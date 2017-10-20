#pragma once

#include <memory>
#include "roulette/geometries/geometry.h"

#include "rapidjson/document.h"

namespace roulette {
  namespace geometries {
    class Circle : public Geometry {
      private:
        const ThreeVector m_center;
        const ThreeVector m_u1;
        const ThreeVector m_u2;

      public:
        static std::shared_ptr<Rectangle> from_json(const rapidjson::Value& data);
        Rectangle(const ThreeVector& p0, const ThreeVector& p1, const ThreeVector& p2);

        ThreeVector sample(RandomGenerator& generator) const;
    };
  };
};
