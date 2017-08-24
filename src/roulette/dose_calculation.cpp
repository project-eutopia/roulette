#include "roulette/dose_calculation.h"
#include "roulette/matrix_three_tensor.h"
#include "roulette/sparse_three_tensor.h"
#include "roulette/pointwise_three_tensor.h"

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

    if (data.HasMember("structure_grid")) {
      m_structure_grid = std::make_shared<MatrixThreeTensor>(data["structure_grid"].GetString());
    }

    if (!data.HasMember("density_compound_map")) throw std::runtime_error("DoseCalculation needs \"density_compound_map\"");
    m_density_compound_map = std::make_shared<const DensityCompoundMap>(data["density_compound_map"], *m_compound_table);

    if (!data.HasMember("phantom")) throw std::runtime_error("DoseCalculation needs \"phantom\"");
    if (data["phantom"].IsString()) {
      m_phantom = std::make_shared<Phantom>(data["phantom"].GetString());
    }
    else {
      m_phantom = std::make_shared<Phantom>(data["phantom"]);
    }
    m_phantom->set_compound_map(*m_density_compound_map);

    m_generator = data.HasMember("seed") ? RandomGenerator(data["seed"].GetInt()) : RandomGenerator();

    if (!data.HasMember("source_doses") || !data["source_doses"].IsArray()) {
      throw std::runtime_error("DoseCalculation needs \"source_doses\"");
    }

    m_dose_tensor = [&]() -> std::shared_ptr<ThreeTensor> {
      if (data.HasMember("dose_storage")) {
        std::string type;

        // Get type of storage
        if (data["dose_storage"].IsString()) {
          type = data["dose_storage"].GetString();
        }
        else if (data["dose_storage"].IsObject() && data["dose_storage"].HasMember("type") && data["dose_storage"]["type"].IsString()) {
          type = data["dose_storage"]["type"].GetString();
        }
        else {
          throw std::runtime_error("Invalid \"dose_storage\" for SourceDose");
        }

        // Build dose matrix based on storage type
        if (type == std::string("matrix")) {
          return std::make_shared<MatrixThreeTensor>(m_phantom->nx(), m_phantom->ny(), m_phantom->nz(), 0);
        }
        else if (type == std::string("sparse")) {
          return std::make_shared<SparseThreeTensor>(m_phantom->nx(), m_phantom->ny(), m_phantom->nz(), 0);
        }
        else if (type == std::string("pointwise")) {
          return std::make_shared<PointwiseThreeTensor>(m_phantom, data["dose_storage"], 0);
        }
        else {
          throw std::runtime_error("Invalid \"dose_storage\" type " + type + " for SourceDose");
        }
      }
      else {
        return std::make_shared<MatrixThreeTensor>(m_phantom->nx(), m_phantom->ny(), m_phantom->nz(), 0);
      }
    };

    const rapidjson::Value& sources = data["source_doses"];
    for (int i = 0; i < sources.Size(); ++i) {
      int seed = m_generator.random_seed();
      const rapidjson::Value& source = sources[i];
      m_source_doses.emplace_back([&]() -> std::shared_ptr<SourceDose> {
        return std::make_shared<SourceDose>(seed, m_compound_table, m_phantom, this->build_dose_tensor(), source);
      });
    }
  }

  DoseCalculation::DoseCalculation(std::string json_filename) :
    DoseCalculation(Json::json_document_from_file_or_string(json_filename))
  {
  }

  std::shared_ptr<const Phantom> DoseCalculation::phantom() const {
    return m_phantom;
  }

  std::shared_ptr<ThreeTensor> DoseCalculation::build_dose_tensor() const {
    return m_dose_tensor();
  }

  std::vector<std::shared_ptr<SourceDose>> DoseCalculation::source_doses() const {
    std::vector<std::shared_ptr<SourceDose>> source_doses;

    for (auto& f : m_source_doses) {
      source_doses.push_back(f());
      source_doses.back()->run();
    }

    return source_doses;
  }

  void DoseCalculation::write_doses() {
    // Run in batches of 4
    int N = 4;
    for (int i = 0; i < m_source_doses.size(); i += N) {
      std::vector<std::thread> threads;

      for (int j = i; j < m_source_doses.size() && j < i+4; ++j) {
        int index = j;
        threads.emplace_back([this, index]() {
          std::shared_ptr<SourceDose> source_dose = this->m_source_doses[index]();
          source_dose->run();

          std::string filename = m_output_folder + "/" + std::string("dose_") + std::to_string(index) + std::string(".dose");
          std::cout << "Writing to file " << filename << std::endl;
          std::ofstream ofs;
          ofs.open(filename, std::ofstream::out);
          source_dose->dose()->write(ofs);
          ofs.close();
        });
      }

      for (auto& t : threads) {
        t.join();
      }
    }
  }
};
