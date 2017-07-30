#include "roulette/source_simulation.h"
#include "roulette/particle.h"
#include "roulette/sources/source_factory.h"

#include <stdexcept>

namespace roulette {
  SourceSimulation::SourceSimulation(unsigned int seed, std::shared_ptr<const CompoundTable> compound_table, std::shared_ptr<const Phantom> phantom, const rapidjson::Value& data) :
    m_generator(seed),
    m_compound_table(compound_table),
    m_phantom(phantom),
    m_dose(m_phantom->nx(), m_phantom->ny(), m_phantom->nz(), 0),
    m_number_of_particles(data["number_of_particles"].GetInt()),
    m_weight(data["weight"].GetDouble()),
    m_source(sources::SourceFactory::source(data["source"])),
    m_finished(false)
  {
  }

  RandomGenerator& SourceSimulation::generator() { return m_generator; }
  const CompoundTable& SourceSimulation::compound_table() const { return *m_compound_table; }
  const Phantom& SourceSimulation::phantom() const { return *m_phantom; }
  ThreeTensor& SourceSimulation::dose() { return m_dose; }

  void SourceSimulation::run() {
    if (m_finished) {
      throw std::runtime_error("Cannot re-run simulation");
    }

    for (int i = 0; i < m_number_of_particles; ++i) {
      std::shared_ptr<Particle> particle = m_source->particle(m_generator);
      particle->deposit_energy(*this);
    }

    // Re-weight by m_weight, and scale from energy to dose
    for (int zi = 0; zi < m_dose.nz(); zi++) {
      for (int yi = 0; yi < m_dose.ny(); yi++) {
        for (int xi = 0; xi < m_dose.nx(); xi++) {
          m_dose(xi, yi, zi) *= m_weight / (*m_phantom)(xi, yi, zi);
        }
      }
    }
  }
};
