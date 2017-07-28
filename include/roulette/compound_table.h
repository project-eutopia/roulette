#pragma once

#include <map>
#include "roulette/periodic_table.h"
#include "roulette/compound.h"

#include "rapidjson/document.h"

namespace roulette {
  class CompoundTable {
    private:
      PeriodicTable m_periodic_table;
      std::map<std::string,Compound> m_compounds;

    public:
      CompoundTable(const rapidjson::Value& data);
      CompoundTable(std::string json_filename);

      const PeriodicTable& periodic_table() const;
      // Name => Compound
      const std::map<std::string,Compound>& compounds() const;
      bool has_compound(std::string name) const;
      const Compound& compound(std::string name) const;
  };
};
