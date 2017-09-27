#pragma once

#include <memory>
#include <string>

#include "roulette/random_generator.h"
#include "roulette/compound_table.h"
#include "roulette/phantom.h"
#include "roulette/source_dose.h"
#include "roulette/three_tensor.h"

#include "rapidjson/document.h"

namespace roulette {
  class DoseCalculation {
    private:
      std::string m_description;
      std::string m_output_folder;
      std::shared_ptr<const CompoundTable> m_compound_table;
      std::shared_ptr<const DensityCompoundMap> m_density_compound_map;
      std::shared_ptr<Phantom> m_phantom;
      std::shared_ptr<Phantom> m_original_phantom;
      RandomGenerator m_generator;

      std::function<std::shared_ptr<ThreeTensor>()> m_dose_tensor;
      std::vector<std::function<std::shared_ptr<SourceDose>()>> m_source_doses;

    public:
      DoseCalculation(const rapidjson::Value& data);
      DoseCalculation(std::string filename_or_json_string);

      std::shared_ptr<const Phantom> phantom() const;
      std::shared_ptr<ThreeTensor> build_dose_tensor() const;
      std::vector<std::shared_ptr<SourceDose>> source_doses() const;

      void write_doses();
  };
};
