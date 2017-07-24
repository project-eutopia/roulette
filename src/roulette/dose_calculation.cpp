#include "roulette/dose_calculation.h"

namespace roulette {
  DoseCalculation::DoseCalculation(std::shared_ptr<RandomGenerator> generator, std::shared_ptr<DensityGrid> density_grid) :
    m_generator(generator),
    m_density_grid(density_grid),
    m_dose(m_density_grid->nz(), m_density_grid->ny(), m_density_grid->nx(), 0.0),
    m_exponential_distribution(1),
    m_compton_scattering()
  {
  }

  void DoseCalculation::process_initial_photon(Photon photon) {
    if (m_density_grid->voxel_grid().outside(photon.position())) {
      m_density_grid->voxel_grid().transport_particle_to_surface(&photon);
    }

    this->process_photon(photon);
  }

  void DoseCalculation::process_photon(Photon& photon) {
    // Roulette to eliminate low energy photons (below 100 keV)
    if (photon.energy() < 100000) {
      // 1 in 3 chance of skipping
      if (m_generator->uniform() > 2.0/3.0) {
        return;
      }
      photon.weight() *= 3.0/2.0;
    }

    // Get random unitless depth
    double depth = m_exponential_distribution(*m_generator);
    bool still_inside = m_density_grid->transport_photon_unitless_depth(&photon, depth);

    if (!still_inside) return;

    // Compton scatter
    m_compton_scattering.set_initial_photon(photon);
    m_compton_scattering(*m_generator);
    Electron electron = photon.compton_scatter(m_compton_scattering.final_photon_energy(), m_compton_scattering.final_electron_energy(), m_compton_scattering.final_photon_theta(), m_compton_scattering.final_electron_theta(), m_compton_scattering.final_phi());

    this->process_photon(photon);
    this->process_electron(electron);
  }

  void DoseCalculation::process_electron(Electron& electron) {
    // TODO
  }
};
