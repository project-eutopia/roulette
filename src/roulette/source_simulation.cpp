#include "roulette/source_simulation.h"
#include <stdexcept>

#include "roulette/sources/source_factory.h"

namespace roulette {
  SourceSimulation::SourceSimulation(unsigned int seed, std::shared_ptr<const Phantom> phantom, const rapidjson::Value& data) :
    m_generator(seed),
    m_phantom(phantom),
    m_dose(m_phantom->nx(), m_phantom->ny(), m_phantom->nz(), 0),
    m_number_of_particles(data["number_of_particles"].GetInt()),
    m_weight(data["weight"].GetDouble()),
    m_source(sources::SourceFactory::source(data["source"])),
    m_finished(false)
  {
  }

  void SourceSimulation::run() {
    if (m_finished) {
      throw std::runtime_error("Cannot re-run simulation");
    }

    for (int i = 0; i < m_number_of_particles; ++i) {
      auto particle = m_source->particle(m_generator);
      particle.deposit_energy(m_phantom, m_dose);
    }

    // TODO: re-weight by m_weight, and scale from energy to dose
  }
};
