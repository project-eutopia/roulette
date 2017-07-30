#pragma once

#include <memory>
#include <string>

#include "roulette/random_generator.h"

#include "rapidjson/document.h"

namespace roulette {
  class CompoundTable;
  class Phantom;
  class SourceSimulation;

  class Simulation {
    private:
      std::string m_description;
      std::shared_ptr<const CompoundTable> m_compound_table;
      std::shared_ptr<Phantom> m_phantom;
      RandomGenerator m_generator;

      std::vector<int> m_number_of_particles;
      std::vector<double> m_weights;
      std::vector<SourceSimulation> m_source_simulations;

    public:
      Simulation(const rapidjson::Value& data);
      Simulation(std::string config_filename);

      void run();
  };
};
