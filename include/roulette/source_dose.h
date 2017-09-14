#pragma once

#include <fstream>
#include <memory>
#include <string>

#include "roulette/random_generator.h"
#include "roulette/compound_table.h"
#include "roulette/phantom.h"
#include "roulette/three_tensor.h"
#include "roulette/sources/source.h"

#include "rapidjson/document.h"

namespace roulette {
  class SourceDose {
    private:
      RandomGenerator m_generator;
      std::shared_ptr<const CompoundTable> m_compound_table;
      std::shared_ptr<const Phantom> m_phantom;
      std::shared_ptr<ThreeTensor> m_dose;

      int m_number_of_particles;
      double m_weight;
      std::shared_ptr<sources::Source> m_source;
      bool m_finished;

    public:
      SourceDose(unsigned int seed, std::shared_ptr<const CompoundTable> compound_table, std::shared_ptr<const Phantom> phantom, std::shared_ptr<ThreeTensor> dose, const rapidjson::Value& data);

      RandomGenerator& generator();
      const CompoundTable& compound_table() const;
      const Phantom& phantom() const;
      std::shared_ptr<ThreeTensor> dose();
      std::shared_ptr<const ThreeTensor> dose() const;
      std::ofstream& write_dose(std::ofstream& os) const;

      void run();
  };
};
