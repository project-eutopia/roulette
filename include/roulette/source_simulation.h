#pragma once

#include <memory>
#include <string>

#include "roulette/phantom.h"
#include "roulette/random_generator.h"
#include "roulette/sources/source.h"

#include "rapidjson/document.h"

namespace roulette {
  class SourceSimulation {
    private:
      RandomGenerator m_generator;
      std::shared_ptr<const Phantom> m_phantom;
      ThreeTensor m_dose;

      int m_number_of_particles;
      double m_weight;
      std::shared_ptr<sources::Source> m_source;
      bool m_finished;

    public:
      SourceSimulation(unsigned int seed, std::shared_ptr<const Phantom> phantom, const rapidjson::Value& data);

      void run();
  };
};
