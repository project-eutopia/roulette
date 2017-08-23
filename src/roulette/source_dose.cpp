#include "roulette/source_dose.h"
#include "roulette/particle.h"
#include "roulette/matrix_three_tensor.h"
#include "roulette/sparse_three_tensor.h"

#include "roulette/sources/source_factory.h"

#include <stdexcept>

namespace roulette {
  SourceDose::SourceDose(unsigned int seed, std::shared_ptr<const CompoundTable> compound_table, std::shared_ptr<const Phantom> phantom, const rapidjson::Value& data) :
    m_generator(seed),
    m_compound_table(compound_table),
    m_phantom(phantom),
    m_number_of_particles(0),
    m_weight(0),
    m_source(sources::SourceFactory::source(data["source"])),
    m_finished(false)
  {
    if (!data["number_of_particles"].IsInt()) throw std::runtime_error("number_of_particles must be integer");
    m_number_of_particles = data["number_of_particles"].GetInt();

    if (!data["weight"].IsNumber()) throw std::runtime_error("weight must be number");
    m_weight = data["weight"].GetDouble();

    if (data.HasMember("dose_storage") && data["dose_storage"].IsString() && data["dose_storage"].GetString() == std::string("sparse")) {
      m_dose = std::make_shared<SparseThreeTensor>(m_phantom->nx(), m_phantom->ny(), m_phantom->nz(), 0);
    }
    else {
      m_dose = std::make_shared<MatrixThreeTensor>(m_phantom->nx(), m_phantom->ny(), m_phantom->nz(), 0);
    }
  }

  RandomGenerator& SourceDose::generator() { return m_generator; }
  const CompoundTable& SourceDose::compound_table() const { return *m_compound_table; }
  const Phantom& SourceDose::phantom() const { return *m_phantom; }
  std::shared_ptr<ThreeTensor> SourceDose::dose() { return m_dose; }

  void SourceDose::run() {
    if (m_finished) {
      throw std::runtime_error("Cannot re-run dose calculation");
    }
    m_finished = true;

    for (int i = 0; i < m_number_of_particles; ++i) {
      std::shared_ptr<Particle> particle = m_source->particle(m_generator);
      particle->deposit_energy(*this);
    }

    m_dose->rescale(m_weight, m_phantom->densities());
  }
};
