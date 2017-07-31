#include "roulette/sources/beamlet_source.h"
#include "roulette/photon.h"

namespace roulette {
  namespace sources {
    BeamletSource::BeamletSource(const rapidjson::Value& data) {
      assert(data["type"].GetString() == std::string("BeamletSource"));

      if (!data.HasMember("source")) throw std::runtime_error("BeamSource requires \"source\"");
      ThreeVector source(data["source"]);

      if (!data.HasMember("bottom_left")) throw std::runtime_error("BeamSource requires \"bottom_left\"");
      ThreeVector bottom_left(data["bottom_left"]);

      if (!data.HasMember("bottom_right")) throw std::runtime_error("BeamSource requires \"bottom_right\"");
      ThreeVector bottom_right(data["bottom_right"]);

      if (!data.HasMember("top_right")) throw std::runtime_error("BeamSource requires \"top_right\"");
      ThreeVector top_right(data["top_right"]);

      m_beamlet = Beamlet(source, bottom_left, bottom_right, top_right);

      if (!data.HasMember("spectrum")) throw std::runtime_error("BeamSource requires \"spectrum\"");
      m_energy_spectrum = distributions::Spectrum(data["spectrum"]);
      if (data["spectrum"].IsString()) {
        m_energy_spectrum = distributions::Spectrum(data["spectrum"].GetString());
      }
      else {
        m_energy_spectrum = distributions::Spectrum(data["spectrum"]);
      }
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
