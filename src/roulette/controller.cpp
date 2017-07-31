#include "roulette/controller.h"

#include "roulette/simulation.h"

namespace roulette {

  Controller::Controller(std::string filename_or_json_string) :
    m_settings(Json::json_document_from_file_or_string(filename_or_json_string))
  {
    if (!m_settings.HasMember("type")) throw std::runtime_error("Settings must define \"type\"");
  }

  std::string Controller::type() const { return m_settings["type"].GetString(); }

  void Controller::run() {
    if (this->type() == std::string("Simulation")) {
      Simulation simulation(m_settings["simulation"]);
      simulation.run();
      simulation.write_doses();
    }

    else if (this->type() == std::string("BeamletOptimization")) {
      // TODO
    }
  }
};
