#pragma once

#include <memory>
#include <string>

#include "roulette/random_generator.h"
#include "roulette/compound_table.h"
#include "roulette/phantom.h"
#include "roulette/source_simulation.h"

#include "rapidjson/document.h"

namespace roulette {
  class ParticleSimulation {
    private:
      std::string m_description;
      std::string m_output_folder;
      std::shared_ptr<const CompoundTable> m_compound_table;
      std::shared_ptr<const DensityCompoundMap> m_density_compound_map;
      std::shared_ptr<Phantom> m_phantom;
      RandomGenerator m_generator;

      std::vector<SourceSimulation> m_source_simulations;

    public:
      ParticleSimulation(const rapidjson::Value& data);
      ParticleSimulation(std::string filename_or_json_string);

      const std::vector<SourceSimulation> source_simulations() const;

      void run();
      void write_simulations();
  };
};
