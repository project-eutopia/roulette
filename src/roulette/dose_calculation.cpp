#include "roulette/dose_calculation.h"

namespace roulette {
  DoseCalculation::DoseCalculation(std::shared_ptr<RandomGenerator> generator, std::shared_ptr<DensityGrid> density_grid) :
    m_generator(generator),
    m_density_grid(density_grid),
    m_dose(m_density_grid->nx(), m_density_grid->ny(), m_density_grid->nz(), 0.0),
    m_exponential_distribution(1),
    m_compton_scattering(),
    m_interaction_function()
  {
  }

  const ThreeTensor& DoseCalculation::dose() const { return m_dose; }

  void DoseCalculation::process_initial_photon(Photon photon) {
    if (m_density_grid->voxel_grid().outside(photon.position())) {
      m_density_grid->voxel_grid().transport_particle_to_surface(&photon);
    }

    this->process_photon(photon);
  }

  void DoseCalculation::process_photon(Photon& photon) {
    // Roulette to eliminate low energy photons (below 100 keV)
    if (photon.energy() < 10000) {
      // 1 in 3 chance of skipping
      if (m_generator->uniform() > 2.0/3.0) {
        return;
      }
      photon.weight() *= 3.0/2.0;
    }

    double energy = photon.energy();

    InteractionType interaction = InteractionType::NONE;

    ThreeVector final_position = m_density_grid->ray_trace_voxels(
      photon.position(), photon.momentum().three_momentum(),
      DensityGrid::voxel_iterator(
        [&](const DensityGrid& cur_density_grid, double distance, int xi, int yi, int zi) -> double {
          // Check for scatter
          if (m_generator->uniform() < m_interaction_function(cur_density_grid(xi, yi, zi) * cur_density_grid.compound().photon_scattering_cross_section(energy) * distance)) {
            interaction = InteractionType::PHOTON_SCATTER;
            return distance / 2.0;
          }
          // Check for absorption
          else if (m_generator->uniform() < m_interaction_function(cur_density_grid(xi, yi, zi) * cur_density_grid.compound().photon_absorption_cross_section(energy) * distance)) {
            interaction = InteractionType::PHOTON_ABSORB;
            // Photoelectric absorption
            m_dose(xi, yi, zi) += photon.weight() * energy;
            return distance / 2.0;
          }

          return distance;
        }
      )
    );

    // Absorption already handled in ray tracer
    if (interaction == InteractionType::PHOTON_SCATTER) {
      photon.position() = final_position;

      m_compton_scattering.set_initial_photon(photon);
      m_compton_scattering(*m_generator);
      Electron electron = photon.compton_scatter(m_compton_scattering.final_photon_energy(), m_compton_scattering.final_electron_energy(), m_compton_scattering.final_photon_theta(), m_compton_scattering.final_electron_theta(), m_compton_scattering.final_phi());

      this->process_electron(electron);
      this->process_photon(photon);
    }
  }

  void DoseCalculation::process_electron(Electron& electron) {
    double kinetic_energy = electron.kinetic_energy();

    ThreeVector final_position = m_density_grid->ray_trace_voxels(
      electron.position(), electron.momentum().three_momentum(),
      DensityGrid::voxel_iterator(
        [&](const DensityGrid& cur_density_grid, double distance, int xi, int yi, int zi) -> double {
          double delta_energy = cur_density_grid(xi, yi, zi) * cur_density_grid.compound().electron_stopping_power(kinetic_energy) * distance;

          double energy_drop = (delta_energy < kinetic_energy) ? delta_energy : kinetic_energy;

          m_dose(xi, yi, zi) += electron.weight() * energy_drop;
          kinetic_energy -= energy_drop;

          // Roulette to decide if we keep going
          if (kinetic_energy < 10000) {
            // 1 in 3 chance of skipping
            if (m_generator->uniform() > 2.0/3.0) {
              return 0;
            }
            electron.weight() *= 3.0/2.0;
          }

          if (energy_drop < kinetic_energy) {
            return distance;
          }

          return energy_drop / cur_density_grid(xi, yi, zi) / cur_density_grid.compound().electron_stopping_power(kinetic_energy);
        }
      )
    );
  }
};
