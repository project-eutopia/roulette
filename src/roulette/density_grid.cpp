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
    bool exited = true;

    ThreeVector final_position = this->ray_trace_voxels(
      photon->position(), photon->momentum().three_momentum(),
      DensityGrid::voxel_iterator(
        [&](const DensityGrid& cur_density_grid, double distance, int xi, int yi, int zi) -> double {
          double delta_depth = cur_density_grid(xi, yi, zi) * cur_density_grid.material().photon_mass_attenuation(photon->energy()) * distance;
          if (delta_depth + current_depth < depth) return distance;

          exited = false;
          return ((delta_depth + current_depth) - depth) / cur_density_grid(xi, yi, zi) / cur_density_grid.material().photon_mass_attenuation(photon->energy());
        }
      )
    );

    photon->position() = final_position;
    return !exited;
  }

  ThreeVector DensityGrid::ray_trace_voxels(const ThreeVector& initial_position, const ThreeVector& direction, DensityGrid::voxel_iterator it) const {
    double mag = direction.magnitude();
    assert(mag > 0);
    ThreeVector u = direction / mag;

    ThreeVector current_position = initial_position;
    // Done if does not intersect surface
    if (m_voxel_grid.outside(current_position) && !m_voxel_grid.transport_position_to_surface(current_position, u)) return initial_position;

    int xinc, yinc, zinc;
    int xi, yi, zi;

    // Coordinates in units of voxel indexes
    double normal_x = (current_position(0) - m_voxel_grid.v0()(0)) / this->delta_x();
    double normal_y = (current_position(1) - m_voxel_grid.v0()(1)) / this->delta_y();
    double normal_z = (current_position(2) - m_voxel_grid.v0()(2)) / this->delta_z();

    // Set increments to +1 for moving forward, -1 for backward, and 0 for stationary
    //
    // Also, the initial voxel index depends on the direction of travel as follows:
    // Moving forward, voxel i has normal coordinate range [i, i+1) (i.e. floor(x))
    // Moving backward, voxel i has normal coordinate range (i, i+1] (i.e. ceil(x-1))
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

    double delta_t = 0;

    // If not incrementing, permanently set time to next voxel along that coordinate to
    // infinity so it is never considered
    double time_to_x = (xinc == 0) ? std::numeric_limits<double>::infinity() : 0;
    double time_to_y = (yinc == 0) ? std::numeric_limits<double>::infinity() : 0;
    double time_to_z = (zinc == 0) ? std::numeric_limits<double>::infinity() : 0;

    while (xi >= 0 && xi < this->nx() && yi >= 0 && yi < this->ny() && zi >= 0 && zi < this->nz()) {
      if (xinc) time_to_x = (m_voxel_grid.v0()(0) + (xi + (xinc > 0))*this->delta_x() - current_position(0)) / u(0);
      if (yinc) time_to_y = (m_voxel_grid.v0()(1) + (yi + (yinc > 0))*this->delta_y() - current_position(1)) / u(1);
      if (zinc) time_to_z = (m_voxel_grid.v0()(2) + (zi + (zinc > 0))*this->delta_z() - current_position(2)) / u(2);

      delta_t = std::min(time_to_x, std::min(time_to_y, time_to_z));

      // Iterator callback
      double distance = it(*this, delta_t, xi, yi, zi);
      // Here we are finished, so return final position
      if (distance < delta_t) return current_position += distance * u;

      // If passing through an edge or corner, will increment multiple indexes here (happens when, e.g., tx = ty < tz)
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
    }

    return current_position;
  }
};
