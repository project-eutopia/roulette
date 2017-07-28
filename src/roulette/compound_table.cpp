#include "roulette/compound_table.h"
#include "roulette/json.h"

namespace roulette {
  CompoundTable::CompoundTable(const rapidjson::Value& data) :
    m_periodic_table(data["elements"])
  {

    const rapidjson::Value& compounds = data["compounds"];
    for (auto it = compounds.Begin(); it != compounds.End(); ++it) {
      std::string name = (*it)["name"].GetString();
      m_compounds.emplace(name, Compound(*it, m_periodic_table));
    }

    for (const auto& atomic_number_element : m_periodic_table.elements()) {
      m_compounds.emplace(atomic_number_element.second.name(), atomic_number_element.second);
    }
  }

  CompoundTable::CompoundTable(std::string json_filename) :
    CompoundTable(Json::json_document_from_file(json_filename))
  {
  }

  const PeriodicTable& CompoundTable::periodic_table() const { return m_periodic_table; }

  // Name => Compound
  const std::map<std::string,Compound>& CompoundTable::compounds() const {
    return m_compounds;
  }

  bool CompoundTable::has_compound(std::string name) const {
    return m_compounds.find(name) != m_compounds.end();
  }

  const Compound& CompoundTable::compound(std::string name) const {
    return m_compounds.at(name);
  }
};
