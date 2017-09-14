#include "roulette/sources/dirac_delta_source.h"
#include "roulette/photon.h"

namespace roulette {
  namespace sources {
    DiracDeltaSource::DiracDeltaSource(const rapidjson::Value& data) {
      assert(data["type"].GetString() == std::string("DiracDeltaSource"));

      if (!data.HasMember("source")) throw std::runtime_error("DiracDeltaSource requires \"source\"");
      m_source = ThreeVector(data["source"]);

      if (!data.HasMember("direction")) throw std::runtime_error("DiracDeltaSource requires \"direction\"");
      m_direction = ThreeVector(data["direction"]);

      if (!data.HasMember("spectrum")) throw std::runtime_error("DiracDeltaSource requires \"spectrum\"");
      m_energy_spectrum = distributions::Spectrum(data["spectrum"]);
      if (data["spectrum"].IsString()) {
        m_energy_spectrum = distributions::Spectrum(data["spectrum"].GetString());
      }
      else {
        m_energy_spectrum = distributions::Spectrum(data["spectrum"]);
      }
    }

    DiracDeltaSource::DiracDeltaSource(const ThreeVector& source, const ThreeVector& direction, const distributions::Spectrum& energy_spectrum) :
      m_source(source),
      m_direction(direction),
      m_energy_spectrum(energy_spectrum)
    {
      m_direction = m_direction / m_direction.magnitude();
    }

    std::shared_ptr<Particle> DiracDeltaSource::particle(RandomGenerator& generator) {
      double energy = m_energy_spectrum(generator);

      return std::make_shared<Photon>(
        FourMomentum(
          energy,
          energy*m_direction(0),
          energy*m_direction(1),
          energy*m_direction(2)
        ),
        m_source
      );
    }
  };
};
