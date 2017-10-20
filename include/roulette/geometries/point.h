#pragma once

#include <memory>
#include "roulette/geometries/geometry.h"

#include "rapidjson/document.h"

namespace roulette {
  namespace geometries {
    class Point : public Geometry {
      private:
        const ThreeVector m_point;

      public:
        static std::shared_ptr<Point> from_json(const rapidjson::Value& data);
        Point(const ThreeVector& three_vector);

        ThreeVector sample(RandomGenerator& generator) const;
    };
  };
};
