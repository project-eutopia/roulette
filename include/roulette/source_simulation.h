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

      int m_number_of_particles;
      double m_weight;
      std::shared_ptr<sources::Source> m_source;

    public:
      SourceSimulation(unsigned int seed, std::shared_ptr<const Phantom> phantom, const rapidjson::Value& data);
  };
};
