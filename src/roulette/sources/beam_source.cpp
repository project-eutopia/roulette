#include "roulette/sources/beam_source.h"

namespace roulette {
  namespace sources {
    BeamSource::BeamSource(const Beam& beam, const distributions::FluenceDistribution& fluence, const distributions::Spectrum& energy_spectrum) :
      m_beam(beam),
      m_fluence(fluence),
      m_energy_spectrum(energy_spectrum)
    {
      assert(m_fluence.nx() == m_beam.nx());
      assert(m_fluence.ny() == m_beam.ny());
    }

    Photon BeamSource::particle(RandomGenerator& generator) {
      double energy = m_energy_spectrum(generator);
      std::pair<int,int> index = m_fluence.index(generator);

      ThreeVector direction = m_beam.directional_unit_vector(generator, index.first, index.second);

      return Photon(
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
