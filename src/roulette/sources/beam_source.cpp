#include "roulette/sources/beam_source.h"
#include "roulette/photon.h"

namespace roulette {
  namespace sources {
    BeamSource::BeamSource(const rapidjson::Value& data) {
      assert(data["type"].GetString() == std::string("BeamSource"));

      ThreeVector source(
        data["source"][0].GetDouble(),
        data["source"][1].GetDouble(),
        data["source"][2].GetDouble()
      );
      ThreeVector bottom_left(
        data["bottom_left"][0].GetDouble(),
        data["bottom_left"][1].GetDouble(),
        data["bottom_left"][2].GetDouble()
      );
      ThreeVector bottom_right(
        data["bottom_right"][0].GetDouble(),
        data["bottom_right"][1].GetDouble(),
        data["bottom_right"][2].GetDouble()
      );
      ThreeVector top_right(
        data["top_right"][0].GetDouble(),
        data["top_right"][1].GetDouble(),
        data["top_right"][2].GetDouble()
      );

      m_beam = Beam(source, bottom_left, bottom_right, top_right, data["nx"].GetInt(), data["ny"].GetInt());
      m_fluence = distributions::FluenceDistribution(TwoTensor(data["fluence"]));
      m_energy_spectrum = distributions::Spectrum(data["spectrum"].GetString());
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
