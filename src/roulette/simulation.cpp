#include "roulette/simulation.h"
#include "roulette/compound_table.h"
#include "roulette/phantom.h"
#include "roulette/source_simulation.h"

#include "roulette/json.h"

namespace roulette {
  Simulation::Simulation(const rapidjson::Value& data)
  {
    m_description = data.HasMember("description") ? data["description"].GetString() : "";

    assert(data.HasMember("compound_table"));
    m_compound_table = std::make_shared<const CompoundTable>(data["compound_table"].GetString());

    assert(data.HasMember("phantom"));
    m_phantom = std::make_shared<Phantom>(data["phantom"].GetString());
    // FIXME: for now, hard code water
    m_phantom->set_compound(m_compound_table->compound("Water, Liquid"));

    m_generator = data.HasMember("seed") ? RandomGenerator(data["seed"].GetInt()) : RandomGenerator();

    const rapidjson::Value& sources = data["sources"];
    for (auto it = sources.Begin(); it != sources.End(); ++it) {
      m_source_simulations.emplace_back(m_generator.random_seed(), m_compound_table, m_phantom, *it);
    }
  }

  Simulation::Simulation(std::string json_filename) :
    Simulation(Json::json_document_from_file(json_filename))
  {
  }

  void Simulation::run() {
    // TODO multi-thread
    for (auto& source_simulation : m_source_simulations) {
      source_simulation.run();
    }
  }
};
