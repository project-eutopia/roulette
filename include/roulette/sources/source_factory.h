#pragma once

#include <memory>

#include "roulette/sources/source.h"

#include "rapidjson/document.h"

namespace roulette {
  namespace sources {
    class Source;

    class SourceFactory {
      public:
        static std::shared_ptr<Source> source(const rapidjson::Value& data);
    };
  };
};
