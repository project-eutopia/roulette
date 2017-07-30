#include "roulette/source_simulation.h"

#include "roulette/sources/source_factory.h"

namespace roulette {
  SourceSimulation::SourceSimulation(unsigned int seed, const rapidjson::Value& data) :
    m_generator(seed)
  {
    m_number_of_particles = data["number_of_particles"].GetInt();
    m_weight = data["weight"].GetDouble();
    m_source = sources::SourceFactory::source(data["source"]);
  }
};
