#include "roulette/density_grid.h"

#include <cassert>

namespace roulette {
  DensityGrid::DensityGrid(const VoxelGrid& voxel_grid, const ThreeTensor& densities, const Material& material) :
    m_voxel_grid(voxel_grid),
    m_densities(densities),
    m_material(material)
  {
  }

  int DensityGrid::nz() const { return m_densities.nz(); }
  int DensityGrid::ny() const { return m_densities.ny(); }
  int DensityGrid::nx() const { return m_densities.nx(); }

  const VoxelGrid& DensityGrid::voxel_grid() const { return m_voxel_grid; }
  double DensityGrid::operator()(int zi, int yi, int xi) const { return m_densities(zi, yi, xi); }
  const Material& DensityGrid::material() const { return m_material; }

  bool DensityGrid::transport_photon_unitless_depth(Photon* photon, double depth) const {
    double current_depth = 0;

    ThreeVector u = photon->position().direction_unit_vector();
    double exit_time = m_voxel_grid.exit_time(*photon);
    // Convert to distance since we using unit vector to move forward
    exit_time *= photon->momentum().momentum_magnitude();

    int xi = (int)((photon->position()(0) - m_voxel_grid.v0()(0)) / m_voxel_grid.delta_x());
    int yi = (int)((photon->position()(1) - m_voxel_grid.v0()(1)) / m_voxel_grid.delta_y());
    int zi = (int)((photon->position()(2) - m_voxel_grid.v0()(2)) / m_voxel_grid.delta_z());

    int xinc, yinc, zinc;

    if (photon->momentum()(1) < 0) xinc = -1;
    else if (photon->momentum()(1) > 0) xinc = 1;
    else xinc = 0;

    if (photon->momentum()(2) < 0) yinc = -1;
    else if (photon->momentum()(2) > 0) yinc = 1;
    else yinc = 0;

    if (photon->momentum()(2) < 0) zinc = -1;
    else if (photon->momentum()(2) > 0) zinc = 1;
    else zinc = 0;

    ThreeVector current_position = photon->position();
    double current_time = 0;

    double time_to_x, time_to_y, time_to_z;
    double current_density = 0;
    double current_attenuation = 0;
    double delta_t = 0;

    while (current_time < exit_time && current_depth < depth) {
      current_density = m_densities(zi, yi, xi);
      current_attenuation = this->material().photon_mass_attenuation(photon->energy());

      if (xinc != 0) time_to_x = std::numeric_limits<double>::infinity();
      else time_to_x = (m_voxel_grid.v0()(0) + (xi + (xinc > 0))*m_voxel_grid.delta_x() - current_position(0)) / u(0);

      if (yinc != 0) time_to_y = std::numeric_limits<double>::infinity();
      else time_to_y = (m_voxel_grid.v0()(1) + (yi + (yinc > 0))*m_voxel_grid.delta_y() - current_position(1)) / u(1);

      if (zinc != 0) time_to_z = std::numeric_limits<double>::infinity();
      else time_to_z = (m_voxel_grid.v0()(2) + (zi + (zinc > 0))*m_voxel_grid.delta_z() - current_position(2)) / u(2);

      if (time_to_x <= std::min(time_to_y, time_to_z)) {
        xi += xinc;
        delta_t = time_to_x;
      }

      if (time_to_y <= std::min(time_to_x, time_to_z)) {
        yi += yinc;
        delta_t = time_to_y;
      }

      if (time_to_z <= std::min(time_to_x, time_to_y)) {
        zi += zinc;
        delta_t = time_to_z;
      }

      /* std::cout << xi << ", " << yi << ", " << zi << std::endl; */
      current_position += delta_t * u;
      current_time += delta_t;

      current_depth += current_density * current_attenuation * delta_t;

    }

    // If overshot, move back a bit
    if (current_depth > depth) {
      delta_t = (current_depth - depth) / current_density / current_attenuation;
      current_time -= delta_t;
      current_position += -delta_t * u;
    }

    photon->position() = current_position;

    return (current_time < exit_time);
  }
};
