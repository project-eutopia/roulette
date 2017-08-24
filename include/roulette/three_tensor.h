#pragma once

#include "roulette/math.h"

#include <vector>
#include <memory>
#include <string>

namespace roulette {
  class ThreeTensor {
    public:
      virtual int nx() const = 0;
      virtual int ny() const = 0;
      virtual int nz() const = 0;

      virtual double operator()(int xi, int yi, int zi) const = 0;
      virtual double& operator()(int xi, int yi, int zi) = 0;

      virtual std::ofstream& write(std::ofstream& os) const = 0;
      virtual std::ifstream& read(std::ifstream& is) = 0;

      virtual void rescale(double weight, std::shared_ptr<const ThreeTensor> densities) = 0;

      double trilinearly_interpolated_value(double normal_x, double normal_y, double normal_z) const {
        int xi = math::floori(normal_x - 0.5);
        int yi = math::floori(normal_y - 0.5);
        int zi = math::floori(normal_z - 0.5);

        // Skip those that completely fall outside
        if (xi < -1 || xi > this->nx()) return 0;
        if (yi < -1 || yi > this->ny()) return 0;
        if (zi < -1 || zi > this->nz()) return 0;

        // dx, dy, dz is a number between 0 and 1, with 0 meaning the weighting
        // is completely towards the lower voxel, and 1 toward the higher voxel.
        double dx = normal_x - 0.5 - xi;
        double dy = normal_y - 0.5 - yi;
        double dz = normal_z - 0.5 - zi;

        return (
          ((xi   >= 0 && xi   < this->nx() && yi   >= 0 && yi   < this->ny() && zi   >= 0 && zi   < this->nz()) ? (*this)(xi,   yi,   zi)   * (1-dx)*(1-dy)*(1-dz) : 0) +
          ((xi+1 >= 0 && xi+1 < this->nx() && yi   >= 0 && yi   < this->ny() && zi   >= 0 && zi   < this->nz()) ? (*this)(xi+1, yi,   zi)   * ( dx )*(1-dy)*(1-dz) : 0) +
          ((xi   >= 0 && xi   < this->nx() && yi+1 >= 0 && yi+1 < this->ny() && zi   >= 0 && zi   < this->nz()) ? (*this)(xi,   yi+1, zi)   * (1-dx)*( dy )*(1-dz) : 0) +
          ((xi+1 >= 0 && xi+1 < this->nx() && yi+1 >= 0 && yi+1 < this->ny() && zi   >= 0 && zi   < this->nz()) ? (*this)(xi+1, yi+1, zi)   * ( dx )*( dy )*(1-dz) : 0) +
          ((xi   >= 0 && xi   < this->nx() && yi   >= 0 && yi   < this->ny() && zi+1 >= 0 && zi+1 < this->nz()) ? (*this)(xi,   yi,   zi+1) * (1-dx)*(1-dy)*( dz ) : 0) +
          ((xi+1 >= 0 && xi+1 < this->nx() && yi   >= 0 && yi   < this->ny() && zi+1 >= 0 && zi+1 < this->nz()) ? (*this)(xi+1, yi,   zi+1) * ( dx )*(1-dy)*( dz ) : 0) +
          ((xi   >= 0 && xi   < this->nx() && yi+1 >= 0 && yi+1 < this->ny() && zi+1 >= 0 && zi+1 < this->nz()) ? (*this)(xi,   yi+1, zi+1) * (1-dx)*( dy )*( dz ) : 0) +
          ((xi+1 >= 0 && xi+1 < this->nx() && yi+1 >= 0 && yi+1 < this->ny() && zi+1 >= 0 && zi+1 < this->nz()) ? (*this)(xi+1, yi+1, zi+1) * ( dx )*( dy )*( dz ) : 0)
        );
      }
  };
};
