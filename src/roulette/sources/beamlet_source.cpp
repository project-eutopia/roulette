#include "roulette/sources/beamlet_source.h"
#include "roulette/photon.h"

namespace roulette {
  namespace sources {
    BeamletSource::BeamletSource(const rapidjson::Value& data) {
      assert(data["type"].GetString() == std::string("BeamletSource"));

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

      m_beamlet = Beamlet(source, bottom_left, bottom_right, top_right);
      m_energy_spectrum = distributions::Spectrum(data["spectrum"].GetString());
    }

    BeamletSource::BeamletSource(const Beamlet& beamlet, const distributions::Spectrum& energy_spectrum) :
      m_beamlet(beamlet),
      m_energy_spectrum(energy_spectrum)
    {
    }

    std::shared_ptr<Particle> BeamletSource::particle(RandomGenerator& generator) {
      double energy = m_energy_spectrum(generator);
      ThreeVector direction = m_beamlet.directional_unit_vector(generator);

      return std::make_shared<Photon>(
        FourMomentum(
          energy,
          energy*direction(0),
          energy*direction(1),
          energy*direction(2)
        ),
        m_beamlet.source()
      );
    }
  };
};
