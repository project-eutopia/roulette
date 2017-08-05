#include "roulette/controller.h"

#include "roulette/dose_calculation.h"
#include "roulette/particle_simulation.h"

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

    if (this->type() == std::string("ParticleSimulation")) {
      ParticleSimulation particle_simulation(m_settings["particle_simulation"]);
      particle_simulation.run();
      particle_simulation.write_simulations();
    }

    else if (this->type() == std::string("BeamletOptimization")) {
      // TODO
    }
  }
};
