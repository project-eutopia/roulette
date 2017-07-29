#include "roulette/sources/beamlet_source.h"

namespace roulette {
  namespace sources {
    BeamletSource::BeamletSource(const Beamlet& beamlet, const distributions::Spectrum& energy_spectrum) :
      m_beamlet(beamlet),
      m_energy_spectrum(energy_spectrum)
    {
    }

    Photon BeamletSource::particle(RandomGenerator& generator) {
      double energy = m_energy_spectrum(generator);
      ThreeVector direction = m_beamlet.directional_unit_vector(generator);

      return Photon(
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
