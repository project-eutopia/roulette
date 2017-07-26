#include "roulette/density_grid.h"
#include "roulette/math.h"

#include <cassert>

namespace roulette {
  DensityGrid::DensityGrid(const VoxelGrid& voxel_grid, const ThreeTensor& densities, const Material& material) :
    m_voxel_grid(voxel_grid),
    m_densities(densities),
    m_material(material)
  {
    m_delta_x = (m_voxel_grid.vn()(0) - m_voxel_grid.v0()(0)) / this->nx();
    m_delta_y = (m_voxel_grid.vn()(1) - m_voxel_grid.v0()(1)) / this->ny();
    m_delta_z = (m_voxel_grid.vn()(2) - m_voxel_grid.v0()(2)) / this->nz();
  }

  int DensityGrid::nz() const { return m_densities.nz(); }
  int DensityGrid::ny() const { return m_densities.ny(); }
  int DensityGrid::nx() const { return m_densities.nx(); }

  double DensityGrid::delta_x() const { return m_delta_x; }
  double DensityGrid::delta_y() const { return m_delta_y; }
  double DensityGrid::delta_z() const { return m_delta_z; }

  const VoxelGrid& DensityGrid::voxel_grid() const { return m_voxel_grid; }
  double DensityGrid::operator()(int xi, int yi, int zi) const { return m_densities(xi, yi, zi); }
  const Material& DensityGrid::material() const { return m_material; }

  bool DensityGrid::transport_photon_unitless_depth(Photon* photon, double depth) const {
    double current_depth = 0;

    ThreeVector u = photon->position().direction_unit_vector();
    double exit_time = m_voxel_grid.exit_time(photon->position(), photon->momentum().three_momentum());
    // Convert to distance since we using unit vector to move forward
    exit_time *= photon->momentum().momentum_magnitude();

    int xi = (int)((photon->position()(0) - m_voxel_grid.v0()(0)) / this->delta_x());
    int yi = (int)((photon->position()(1) - m_voxel_grid.v0()(1)) / this->delta_y());
    int zi = (int)((photon->position()(2) - m_voxel_grid.v0()(2)) / this->delta_z());

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

      if (xinc == 0) time_to_x = std::numeric_limits<double>::infinity();
      else time_to_x = (m_voxel_grid.v0()(0) + (xi + (xinc > 0))*this->delta_x() - current_position(0)) / u(0);

      if (yinc == 0) time_to_y = std::numeric_limits<double>::infinity();
      else time_to_y = (m_voxel_grid.v0()(1) + (yi + (yinc > 0))*this->delta_y() - current_position(1)) / u(1);

      if (zinc == 0) time_to_z = std::numeric_limits<double>::infinity();
      else time_to_z = (m_voxel_grid.v0()(2) + (zi + (zinc > 0))*this->delta_z() - current_position(2)) / u(2);

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

  ThreeVector DensityGrid::ray_trace_voxels(const ThreeVector& initial_position, const ThreeVector& direction, DensityGrid::voxel_iterator it) const {
    ThreeVector u = direction / direction.magnitude();
    ThreeVector current_position = initial_position;
    // Done if does not intersect surface
    if (m_voxel_grid.outside(current_position) && !m_voxel_grid.transport_position_to_surface(current_position, u)) return initial_position;

    double exit_time = m_voxel_grid.exit_time(current_position, u);

    int xinc, yinc, zinc;
    int xi, yi, zi;

    double normal_x = (current_position(0) - m_voxel_grid.v0()(0)) / this->delta_x();
    double normal_y = (current_position(1) - m_voxel_grid.v0()(1)) / this->delta_y();
    double normal_z = (current_position(2) - m_voxel_grid.v0()(2)) / this->delta_z();

    if (u(0) < 0) {
      xinc = -1;
      xi = ceili(normal_x-1);
    }
    else if (u(0) > 0) {
      xinc = 1;
      xi = floori(normal_x);
    }
    else {
      xinc = 0;
      xi = floori(normal_x);
    }

    if (u(1) < 0) {
      yinc = -1;
      yi = ceili(normal_y-1);
    }
    else if (u(1) > 0) {
      yinc = 1;
      yi = floori(normal_y);
    }
    else {
      yinc = 0;
      yi = floori(normal_y);
    }

    if (u(2) < 0) {
      zinc = -1;
      zi = ceili(normal_z-1);
    }
    else if (u(2) > 0) {
      zinc = 1;
      zi = floori(normal_z);
    }
    else {
      zinc = 0;
      zi = floori(normal_z);
    }

    double current_time = 0;
    double delta_t = 0;

    double time_to_x = (xinc == 0) ? std::numeric_limits<double>::infinity() : 0;
    double time_to_y = (yinc == 0) ? std::numeric_limits<double>::infinity() : 0;
    double time_to_z = (zinc == 0) ? std::numeric_limits<double>::infinity() : 0;

    while (current_time < exit_time) {
      if (xinc) time_to_x = (m_voxel_grid.v0()(0) + (xi + (xinc > 0))*this->delta_x() - current_position(0)) / u(0);
      if (yinc) time_to_y = (m_voxel_grid.v0()(1) + (yi + (yinc > 0))*this->delta_y() - current_position(1)) / u(1);
      if (zinc) time_to_z = (m_voxel_grid.v0()(2) + (zi + (zinc > 0))*this->delta_z() - current_position(2)) / u(2);

      delta_t = std::min(time_to_x, std::min(time_to_y, time_to_z));

      double distance = it(*this, delta_t, xi, yi, zi);
      // Here we are finished, so return final position
      if (distance < delta_t) return current_position += distance * u;

      if (time_to_x <= std::min(time_to_y, time_to_z)) {
        xi += xinc;
      }

      if (time_to_y <= std::min(time_to_x, time_to_z)) {
        yi += yinc;
      }

      if (time_to_z <= std::min(time_to_x, time_to_y)) {
        zi += zinc;
      }

      current_position += delta_t * u;
      current_time += delta_t;
    }

    return current_position;
  }
};
