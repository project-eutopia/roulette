#include "roulette/density_compound_map.h"

#include "roulette/json.h"

namespace roulette {
  DensityCompoundMap::DensityCompoundMap(const Compound& compound) {
    m_density_ranges.reserve(1);
    m_compounds.reserve(1);

    m_density_ranges.push_back(std::make_pair(-std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity()));
    m_compounds.push_back(std::make_shared<Compound>(compound));
  }

  DensityCompoundMap::DensityCompoundMap(const rapidjson::Value& data, const CompoundTable& table) {
    if (data.IsString()) {
      m_density_ranges.reserve(1);
      m_compounds.reserve(1);

      m_density_ranges.push_back(std::make_pair(-std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity()));
      m_compounds.push_back(std::make_shared<Compound>(table.compound(data.GetString())));
    }
    else if (data.IsArray()) {
      double low, high;
      for (auto it = data.Begin(); it != data.End(); ++it) {
        if (it->HasMember("low")) {
          if (!(*it)["low"].IsNumber()) throw std::runtime_error("DensityCompoundMap \"low\" must be a number");
          low = (*it)["low"].GetDouble();
        }
        else {
          low = -std::numeric_limits<double>::infinity();
        }

        if (it->HasMember("high")) {
          if (!(*it)["high"].IsNumber()) throw std::runtime_error("DensityCompoundMap \"high\" must be a number");
          high = (*it)["high"].GetDouble();
        }
        else {
          high = std::numeric_limits<double>::infinity();
        }

        if (!m_density_ranges.empty() && m_density_ranges.back().second != low) {
          throw std::runtime_error("DensityCompoundMap density ranges must have no gaps");
        }
        if (low >= high) {
          throw std::runtime_error("DensityCompoundMap density ranges must have \"low\" < \"high\"");
        }

        if (!it->HasMember("compound") || !(*it)["compound"].IsString()) {
          throw std::runtime_error("DensityCompoundMap \"compound\" must be a string");
        }

        m_density_ranges.push_back(std::make_pair(low, high));
        m_compounds.push_back(std::make_shared<Compound>(table.compound((*it)["compound"].GetString())));
      }
    }
    else {
      throw std::runtime_error("DensityCompoundMap needs to be a string or array");
    }
  }

  DensityCompoundMap::DensityCompoundMap(std::string filename_or_json_string, const CompoundTable& table) :
    DensityCompoundMap(Json::json_document_from_file_or_string(filename_or_json_string), table)
  {
  }

  std::shared_ptr<Compound> DensityCompoundMap::compound_for_density(double density) const {
    for (int i = 0; i < m_density_ranges.size(); ++i) {
      if (density >= m_density_ranges[i].first && density < m_density_ranges[i].second) {
        return m_compounds[i];
      }
    }

    throw std::runtime_error("Unknown compound for density");
    // Return blank pointer
    return std::shared_ptr<Compound>();
  }
}
