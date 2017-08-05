#include "roulette/dose_calculation.h"
#include <fstream>
#include <thread>

#include "roulette/json.h"

namespace roulette {
  DoseCalculation::DoseCalculation(const rapidjson::Value& data)
  {
    m_description = data.HasMember("description") ? data["description"].GetString() : "";
    m_output_folder = data.HasMember("output_folder") ? data["output_folder"].GetString() : "./";

    if (!data.HasMember("compound_table")) throw std::runtime_error("DoseCalculation needs \"compound_table\"");
    m_compound_table = std::make_shared<const CompoundTable>(data["compound_table"].GetString());

    if (!data.HasMember("phantom")) throw std::runtime_error("DoseCalculation needs \"phantom\"");
    if (data["phantom"].IsString()) {
      m_phantom = std::make_shared<Phantom>(data["phantom"].GetString());
      // FIXME: for now, hard code water, later use DensityCompoundMap
      m_phantom->set_compound(m_compound_table->compound("Water, Liquid"));
    }
    else {
      m_phantom = std::make_shared<Phantom>(data["phantom"]);
    }

    m_generator = data.HasMember("seed") ? RandomGenerator(data["seed"].GetInt()) : RandomGenerator();

    if (!data.HasMember("source_doses") || !data["source_doses"].IsArray()) {
      throw std::runtime_error("DoseCalculation needs \"source_doses\"");
    }
    const rapidjson::Value& sources = data["source_doses"];
    for (auto it = sources.Begin(); it != sources.End(); ++it) {
      m_source_doses.emplace_back(m_generator.random_seed(), m_compound_table, m_phantom, *it);
    }
  }

  DoseCalculation::DoseCalculation(std::string json_filename) :
    DoseCalculation(Json::json_document_from_file_or_string(json_filename))
  {
  }

  const std::vector<SourceDose> DoseCalculation::source_doses() const { return m_source_doses; }

  void DoseCalculation::run() {
    std::vector<std::thread> threads;
    for (auto& source_dose : m_source_doses) {
      threads.emplace_back([&]() {
        source_dose.run();
      });
    }

    for (auto& t : threads) {
      t.join();
    }
  }

  void DoseCalculation::write_doses() {
    for (int i = 0; i < m_source_doses.size(); ++i) {
      const auto& source_dose = m_source_doses[i];
      std::string filename = m_output_folder + "/" + std::string("dose_") + std::to_string(i) + std::string(".dose");
      std::ofstream ofs;
      ofs.open(filename, std::ofstream::out);
      source_dose.dose().write(ofs);
      ofs.close();
    }
  }
};
