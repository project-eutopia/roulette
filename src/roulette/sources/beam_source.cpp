#include "roulette/sources/beam_source.h"
#include "roulette/photon.h"

#include <stdexcept>

namespace roulette {
  namespace sources {
    BeamSource::BeamSource(const rapidjson::Value& data) {
      assert(data["type"].GetString() == std::string("BeamSource"));

      if (!data.HasMember("source")) throw std::runtime_error("BeamSource requires \"source\"");
      ThreeVector source(data["source"]);

      if (!data.HasMember("bottom_left")) throw std::runtime_error("BeamSource requires \"bottom_left\"");
      ThreeVector bottom_left(data["bottom_left"]);

      if (!data.HasMember("bottom_right")) throw std::runtime_error("BeamSource requires \"bottom_right\"");
      ThreeVector bottom_right(data["bottom_right"]);

      if (!data.HasMember("top_right")) throw std::runtime_error("BeamSource requires \"top_right\"");
      ThreeVector top_right(data["top_right"]);

      if (!data.HasMember("nx") || !data["nx"].IsInt()) throw std::runtime_error("BeamSource requires integer \"nx\"");
      if (!data.HasMember("ny") || !data["ny"].IsInt()) throw std::runtime_error("BeamSource requires integer \"ny\"");
      m_beam = Beam(source, bottom_left, bottom_right, top_right, data["nx"].GetInt(), data["ny"].GetInt());

      if (!data.HasMember("fluence")) throw std::runtime_error("BeamSource requires member \"fluence\"");
      m_fluence = distributions::FluenceDistribution(TwoTensor(data["fluence"]));

      if (!data.HasMember("spectrum")) throw std::runtime_error("BeamSource requires \"spectrum\"");
      m_energy_spectrum = distributions::Spectrum(data["spectrum"]);
      if (data["spectrum"].IsString()) {
        m_energy_spectrum = distributions::Spectrum(data["spectrum"].GetString());
      }
      else {
        m_energy_spectrum = distributions::Spectrum(data["spectrum"]);
      }
    }

    BeamSource::BeamSource(const Beam& beam, const distributions::FluenceDistribution& fluence, const distributions::Spectrum& energy_spectrum) :
      m_beam(beam),
      m_fluence(fluence),
      m_energy_spectrum(energy_spectrum)
    {
      assert(m_fluence.nx() == m_beam.nx());
      assert(m_fluence.ny() == m_beam.ny());
    }

    std::shared_ptr<Particle> BeamSource::particle(RandomGenerator& generator) {
      double energy = m_energy_spectrum(generator);
      std::pair<int,int> index = m_fluence.index(generator);

      ThreeVector direction = m_beam.directional_unit_vector(generator, index.first, index.second);

      return std::make_shared<Photon>(
        FourMomentum(
          energy,
          energy*direction(0),
          energy*direction(1),
          energy*direction(2)
        ),
        m_beam.source()
      );
    }
  };
};
