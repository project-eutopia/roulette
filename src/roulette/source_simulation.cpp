#include "roulette/source_simulation.h"
#include "roulette/particle.h"
#include "roulette/sources/source_factory.h"

#include <stdexcept>

namespace roulette {
  SourceSimulation::SourceSimulation(unsigned int seed, std::shared_ptr<const CompoundTable> compound_table, std::shared_ptr<const Phantom> phantom, const rapidjson::Value& data) :
    m_simulation(),
    m_generator(seed),
    m_compound_table(compound_table),
    m_phantom(phantom),
    m_number_of_particles(data["number_of_particles"].GetInt()),
    m_weight(data["weight"].GetDouble()),
    m_source(sources::SourceFactory::source(data["source"])),
    m_finished(false)
  {
  }

  RandomGenerator& SourceSimulation::generator() { return m_generator; }
  const CompoundTable& SourceSimulation::compound_table() const { return *m_compound_table; }
  const Phantom& SourceSimulation::phantom() const { return *m_phantom; }

  void SourceSimulation::run() {
    if (m_finished) {
      throw std::runtime_error("Cannot re-run source simulation");
    }
    m_finished = true;

    for (int i = 0; i < m_number_of_particles; ++i) {
      std::shared_ptr<Particle> particle = m_source->particle(m_generator);
      m_simulation.add_event(particle->simulate(m_generator, *m_phantom));
    }
  }
};
