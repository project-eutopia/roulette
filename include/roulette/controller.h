#pragma once

#include <string>
#include "roulette/json.h"
#include "rapidjson/document.h"

namespace roulette {
  class Controller {
    private:
      const rapidjson::Document m_settings;

    public:
      Controller(std::string filename_or_json_string);
      void run();

    private:
      std::string type() const;
  };
};
