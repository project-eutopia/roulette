#pragma once

#include <memory>
#include <string>
#include <fstream>

#include "roulette/random_generator.h"
#include "roulette/compound_table.h"
#include "roulette/phantom.h"
#include "roulette/three_tensor.h"
#include "roulette/sources/source.h"

#include "roulette/simulation.h"
#include "rapidjson/document.h"

namespace roulette {
  class SourceSimulation {
    private:
      Simulation m_simulation;
      RandomGenerator m_generator;
      std::shared_ptr<const CompoundTable> m_compound_table;
      std::shared_ptr<const Phantom> m_phantom;

      int m_number_of_particles;
      double m_weight;
      std::shared_ptr<sources::Source> m_source;
      bool m_finished;

    public:
      SourceSimulation(unsigned int seed, std::shared_ptr<const CompoundTable> compound_table, std::shared_ptr<const Phantom> phantom, const rapidjson::Value& data);

      RandomGenerator& generator();
      const CompoundTable& compound_table() const;
      const Phantom& phantom() const;

      void run();

      std::ofstream& write_json(std::ofstream& ofs) const;
  };
};
