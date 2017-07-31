#include "roulette/sources/beamlet_source.h"
#include "roulette/photon.h"

namespace roulette {
  namespace sources {
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
