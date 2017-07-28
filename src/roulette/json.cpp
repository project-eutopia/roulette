#include "roulette/json.h"

#include <fstream>

namespace roulette {
  std::string Json::json_string_from_file(std::string filename) {
    std::ifstream json_file(filename);
    std::string json_content( (std::istreambuf_iterator<char>(json_file) ),
                              (std::istreambuf_iterator<char>()    ) );

    return json_content;
  }

  rapidjson::Document Json::json_document_from_file(std::string filename) {
    rapidjson::Document data;
    data.Parse(Json::json_string_from_file(filename).c_str());
    return data;
  }
};
