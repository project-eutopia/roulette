#include "roulette/simulation.h"
#include <fstream>

#include "roulette/json.h"

namespace roulette {
  Simulation::Simulation(const rapidjson::Value& data)
  {
    m_description = data.HasMember("description") ? data["description"].GetString() : "";

    assert(data.HasMember("compound_table"));
    m_compound_table = std::make_shared<const CompoundTable>(data["compound_table"].GetString());

    assert(data.HasMember("phantom"));
    if (data["phantom"].IsString()) {
      m_phantom = std::make_shared<Phantom>(data["phantom"].GetString());
      // FIXME: for now, hard code water, later use DensityCompoundMap
      m_phantom->set_compound(m_compound_table->compound("Water, Liquid"));
    }
    else {
      m_phantom = std::make_shared<Phantom>(data["phantom"]);
    }

    m_generator = data.HasMember("seed") ? RandomGenerator(data["seed"].GetInt()) : RandomGenerator();

    const rapidjson::Value& sources = data["sources"];
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
      std::string filename = std::string("dose_") + std::to_string(i) + std::string(".dose");
      std::ofstream ofs;
      ofs.open(filename, std::ofstream::out);
      source_simulation.dose().write(ofs);
      ofs.close();
    }
  }
};
