#pragma once

#include <string>
#include "rapidjson/document.h"

namespace roulette {
  class Json {
    public:
      static std::string json_string_from_file(std::string filename);
      static rapidjson::Document json_document_from_file(std::string filename);
  };
};
