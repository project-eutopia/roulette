#include "roulette/periodic_table.h"

namespace roulette {
  PeriodicTable::PeriodicTable(const rapidjson::Value& data)
  {
    int atomic_number;
    for (auto it = data.Begin(); it != data.End(); ++it) {
      atomic_number = (*it)["atomic_number"].GetInt();
      m_elements.emplace(atomic_number, *it);
    }
  }

  const std::map<int,Element>& PeriodicTable::elements() const { return m_elements; }
  const Element& PeriodicTable::element(int atomic_number) const { return m_elements.at(atomic_number); }
};
