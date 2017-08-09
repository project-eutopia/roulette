#pragma once

#include <vector>
#include <memory>

#include "roulette/compound.h"
#include "roulette/compound_table.h"
#include "rapidjson/document.h"

namespace roulette {
  class DensityCompoundMap {
    private:
      std::vector<std::pair<double,double>> m_density_ranges;
      std::vector<std::shared_ptr<Compound>> m_compounds;

    public:
      DensityCompoundMap(const rapidjson::Value& data, const CompoundTable& table);
      DensityCompoundMap(std::string filename_or_json_string, const CompoundTable& table);

      std::shared_ptr<Compound> compound_for_density(double density) const;
  };
}
