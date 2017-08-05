#include "roulette/controller.h"

#include "roulette/dose_calculation.h"

namespace roulette {

  Controller::Controller(std::string filename_or_json_string) :
    m_settings(Json::json_document_from_file_or_string(filename_or_json_string))
  {
    if (!m_settings.HasMember("type")) throw std::runtime_error("Settings must define \"type\"");
  }

  std::string Controller::type() const { return m_settings["type"].GetString(); }

  void Controller::run() {
    if (this->type() == std::string("DoseCalculation")) {
      DoseCalculation dose_calculation(m_settings["dose_calculation"]);
      dose_calculation.run();
      dose_calculation.write_doses();
    }

    else if (this->type() == std::string("BeamletOptimization")) {
      // TODO
    }
  }
};
