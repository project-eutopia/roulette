#pragma once

#include <functional>

#include "roulette/three_vector.h"

namespace roulette {
  class Particle;
  class Phantom;

  class IVoxelGrid {
    public:
      // voxel_iterator is a function that is used to iterate over voxels
      // along a given ray.
      //
      // The function argument distance is the maximum distance to be traveled in this voxel.
      // The return value of this functionshould be the distance processed within this voxel,
      // and if this is less than the maximum distance, then voxel iteration terminates.
      //
      // distance is the maximum distance to traverse in this voxel
      // zi, yi, xi are the indexes of this voxel
      typedef std::function<double(double distance, int xi, int yi, int zi)> voxel_iterator;

    public:
      virtual int nx() const = 0;
      virtual int ny() const = 0;
      virtual int nz() const = 0;

      virtual bool inside(const ThreeVector& point) const = 0;
      virtual bool strictly_inside(const ThreeVector& point) const = 0;
      virtual bool outside(const ThreeVector& point) const = 0;

      // Returns true intersects surface,
      // and therefore was successfully moved up to surface
      virtual bool transport_particle_to_surface(Particle* particle) const = 0;
      virtual bool transport_position_to_surface(ThreeVector& position, const ThreeVector& direction) const = 0;

      // Must be inside and aiming inside to begin with
      virtual double exit_time(const ThreeVector& position, const ThreeVector& velocity) const = 0;

      virtual std::ofstream& write(std::ofstream& os) const = 0;
      virtual std::ifstream& read(std::ifstream& is) = 0;
      virtual rapidjson::Value to_json(rapidjson::Document::AllocatorType& allocator) const = 0;

      // For processing voxels in a ray, where the processing is done by the voxel_iterator.
      // Returns final position
      virtual ThreeVector ray_trace_voxels(const ThreeVector& initial_position, const ThreeVector& direction, voxel_iterator it) const = 0;
  };
};
