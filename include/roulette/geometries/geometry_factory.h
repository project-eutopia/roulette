#pragma once

#include <memory>

#include "roulette/geometries/geometry.h"
#include "rapidjson/document.h"

namespace roulette {
  namespace geometries {
    class GeometryFactory {
      public:
        static std::shared_ptr<Geometry> geometry(const rapidjson::Value& data);
    };
  };
};
