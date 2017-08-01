#include "roulette/simulation.h"
#include <fstream>

#include "roulette/json.h"

namespace roulette {
  Simulation::Simulation(const rapidjson::Value& data)
  {
    m_description = data.HasMember("description") ? data["description"].GetString() : "";
    m_output_folder = data.HasMember("output_folder") ? data["output_folder"].GetString() : "./";

    if (!data.HasMember("compound_table")) throw std::runtime_error("Simulation needs \"compound_table\"");
    m_compound_table = std::make_shared<const CompoundTable>(data["compound_table"].GetString());

    if (!data.HasMember("phantom")) throw std::runtime_error("Simulation needs \"phantom\"");
    if (data["phantom"].IsString()) {
      m_phantom = std::make_shared<Phantom>(data["phantom"].GetString());
      // FIXME: for now, hard code water, later use DensityCompoundMap
      m_phantom->set_compound(m_compound_table->compound("Water, Liquid"));
    }
    else {
      m_phantom = std::make_shared<Phantom>(data["phantom"]);
    }

    m_generator = data.HasMember("seed") ? RandomGenerator(data["seed"].GetInt()) : RandomGenerator();

    if (!data.HasMember("source_simulations") || !data["source_simulations"].IsArray()) {
      throw std::runtime_error("Simulation needs \"source_simulations\"");
    }
    const rapidjson::Value& sources = data["source_simulations"];
    for (auto it = sources.Begin(); it != sources.End(); ++it) {
      m_source_simulations.emplace_back(m_generator.random_seed(), m_compound_table, m_phantom, *it);
    }
  }

  Simulation::Simulation(std::string json_filename) :
    Simulation(Json::json_document_from_file_or_string(json_filename))
  {
  }

  const std::vector<SourceSimulation> Simulation::source_simulations() const { return m_source_simulations; }

  void Simulation::run() {
    // TODO multi-thread
    for (auto& source_simulation : m_source_simulations) {
      source_simulation.run();
    }
  }

  void Simulation::write_doses() {
    for (int i = 0; i < m_source_simulations.size(); ++i) {
      const auto& source_simulation = m_source_simulations[i];
      std::string filename = m_output_folder + "/" + std::string("dose_") + std::to_string(i) + std::string(".dose");
      std::ofstream ofs;
      ofs.open(filename, std::ofstream::out);
      source_simulation.dose().write(ofs);
      ofs.close();
    }
  }
};
