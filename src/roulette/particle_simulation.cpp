#include "roulette/particle_simulation.h"
#include <fstream>

#include "roulette/json.h"

namespace roulette {
  ParticleSimulation::ParticleSimulation(const rapidjson::Value& data)
  {
    m_description = data.HasMember("description") ? data["description"].GetString() : "";
    m_output_folder = data.HasMember("output_folder") ? data["output_folder"].GetString() : "./";

    if (!data.HasMember("compound_table")) throw std::runtime_error("ParticleSimulation needs \"compound_table\"");
    m_compound_table = std::make_shared<const CompoundTable>(data["compound_table"].GetString());

    if (!data.HasMember("phantom")) throw std::runtime_error("ParticleSimulation needs \"phantom\"");
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
      throw std::runtime_error("ParticleSimulation needs \"source_simulations\"");
    }
    const rapidjson::Value& sources = data["source_simulations"];
    for (auto it = sources.Begin(); it != sources.End(); ++it) {
      m_source_simulations.emplace_back(m_generator.random_seed(), m_compound_table, m_phantom, *it);
    }
  }

  ParticleSimulation::ParticleSimulation(std::string json_filename) :
    ParticleSimulation(Json::json_document_from_file_or_string(json_filename))
  {
  }

  const std::vector<SourceSimulation> ParticleSimulation::source_simulations() const { return m_source_simulations; }

  void ParticleSimulation::run() {
    // TODO multi-thread
    for (auto& source_simulation : m_source_simulations) {
      source_simulation.run();
    }
  }

  void ParticleSimulation::write_simulations() {
    // TODO
  }
};
