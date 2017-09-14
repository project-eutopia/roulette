#include "roulette/dose_calculation.h"
#include "roulette/matrix_three_tensor.h"
#include "roulette/sparse_three_tensor.h"
#include "roulette/pointwise_three_tensor.h"
#include "roulette/job_queue.h"

#include <fstream>

#include "roulette/json.h"

namespace roulette {
  DoseCalculation::DoseCalculation(const rapidjson::Value& data)
  {
    m_description = data.HasMember("description") ? data["description"].GetString() : "";
    m_output_folder = data.HasMember("output_folder") ? data["output_folder"].GetString() : "./";

    if (!data.HasMember("compound_table")) throw std::runtime_error("DoseCalculation needs \"compound_table\"");
    m_compound_table = std::make_shared<const CompoundTable>(data["compound_table"].GetString());

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

    if (data.HasMember("phantom_voxelation")) {
      if (!data["phantom_voxelation"].IsArray() || data["phantom_voxelation"].Size() != 3) {
        throw std::runtime_error("DoseCalculation \"phantom_voxelation\" must be an array of 3 positive integers");
      }

      std::tuple<int,int,int> voxelation;
      std::get<0>(voxelation) = data["phantom_voxelation"][0].GetInt();
      std::get<1>(voxelation) = data["phantom_voxelation"][1].GetInt();
      std::get<2>(voxelation) = data["phantom_voxelation"][2].GetInt();

      m_phantom = std::make_shared<Phantom>(*m_phantom, voxelation);
    }

    m_generator = data.HasMember("seed") ? RandomGenerator(data["seed"].GetInt()) : RandomGenerator();

    if (!data.HasMember("source_doses") || !data["source_doses"].IsArray()) {
      throw std::runtime_error("DoseCalculation needs \"source_doses\"");
    }

    std::string type;

    if (data.HasMember("dose_storage")) {
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
    }
    else {
      type = std::string("matrix");
    }

    // Build dose matrix based on storage type
    if (type == std::string("matrix")) {
      m_dose_tensor = [this]() -> std::shared_ptr<ThreeTensor> {
        return std::make_shared<MatrixThreeTensor>(m_phantom->nx(), m_phantom->ny(), m_phantom->nz(), 0);
      };
    }
    else if (type == std::string("sparse")) {
      m_dose_tensor = [this]() -> std::shared_ptr<ThreeTensor> {
        return std::make_shared<SparseThreeTensor>(m_phantom->nx(), m_phantom->ny(), m_phantom->nz(), 0);
      };
    }
    else if (type == std::string("pointwise")) {
      const rapidjson::Value& dose_storage = data["dose_storage"];
      m_dose_tensor = [this, &dose_storage]() -> std::shared_ptr<ThreeTensor> {
        return std::make_shared<PointwiseThreeTensor>(m_phantom, dose_storage, 0);
      };
    }
    else {
      throw std::runtime_error("Invalid \"dose_storage\" type " + type + " for SourceDose");
    }

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
    JobQueue queue;
    for (int i = 0; i < m_source_doses.size(); ++i) {
      int index = i;
      queue.add_job([this, index]() {
        std::shared_ptr<SourceDose> source_dose = this->m_source_doses[index]();
        source_dose->run();

        std::string filename = m_output_folder + "/" + std::string("dose_") + std::to_string(index) + std::string(".dose");
        std::ofstream ofs;
        ofs.open(filename, std::ofstream::out);
        source_dose->write_dose(ofs);
        ofs.close();
      });
    }
    queue.run();
  }
};
