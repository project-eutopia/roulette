#pragma once

#include <memory>
#include <string>

#include "roulette/random_generator.h"
#include "roulette/compound_table.h"
#include "roulette/phantom.h"
#include "roulette/source_dose.h"

#include "rapidjson/document.h"

namespace roulette {
  class DoseCalculation {
    private:
      std::string m_description;
      std::string m_output_folder;
      std::shared_ptr<const CompoundTable> m_compound_table;
      std::shared_ptr<Phantom> m_phantom;
      std::shared_ptr<ThreeTensor> m_structure_grid;
      RandomGenerator m_generator;

      std::vector<SourceDose> m_source_doses;

    public:
      DoseCalculation(const rapidjson::Value& data);
      DoseCalculation(std::string filename_or_json_string);

      const std::vector<SourceDose> source_doses() const;

      void run();
      void write_doses();
  };
};
