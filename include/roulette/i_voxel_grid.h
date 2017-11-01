#pragma once

#include <functional>

#include "roulette/three_vector.h"

namespace roulette {
  class Particle;
  class Phantom;

  class InvalidVoxelGrid : public std::runtime_error {
    public:
      InvalidVoxelGrid(const std::string& s) : std::runtime_error(s) {}
  };

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
      typedef std::function<double(double distance, size_t xi, size_t yi, size_t zi)> voxel_iterator;

    public:
      virtual ~IVoxelGrid() {};

      virtual size_t nx() const = 0;
      virtual size_t ny() const = 0;
      virtual size_t nz() const = 0;

      virtual const ThreeVector& v0() const = 0;
      virtual const ThreeVector& vn() const = 0;

      virtual std::tuple<size_t,size_t,size_t> index_at(const ThreeVector& position) const = 0;

      bool inside(const ThreeVector& point) const;
      bool strictly_inside(const ThreeVector& point) const;
      bool outside(const ThreeVector& point) const;

      // Returns true intersects surface,
      // and therefore was successfully moved up to surface
      bool transport_particle_to_surface(Particle* particle) const;
      bool transport_position_to_surface(ThreeVector& position, const ThreeVector& direction) const;

      // Must be inside and aiming inside to begin with
      double exit_time(const ThreeVector& position, const ThreeVector& velocity) const;

      virtual rapidjson::Value to_json(rapidjson::Document::AllocatorType& allocator) const = 0;

      // For processing voxels in a ray, where the processing is done by the voxel_iterator.
      // Returns final position
      virtual ThreeVector ray_trace_voxels(const ThreeVector& initial_position, const ThreeVector& direction, voxel_iterator it) const = 0;
  };
};
