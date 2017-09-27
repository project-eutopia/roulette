#pragma once

#include <array>

#include "roulette/three_vector.h"
#include "roulette/four_momentum.h"

namespace roulette {
  class RotationMatrix {
    private:
      std::array<std::array<double, 3>, 3> m_matrix;

    public:
      RotationMatrix();
      RotationMatrix(const std::array<std::array<double,3>,3>& matrix);
      RotationMatrix(
        double m00, double m01, double m02,
        double m10, double m11, double m12,
        double m20, double m21, double m22
      );

      double operator()(int i, int j) const;

      static RotationMatrix rotationX(double angle);
      static RotationMatrix rotationY(double angle);
      static RotationMatrix rotationZ(double angle);
      static RotationMatrix rotationU(double angle, const ThreeVector& u);
      static RotationMatrix rotationUtoV(const ThreeVector& u, const ThreeVector& v);

      RotationMatrix inv() const;

      friend RotationMatrix operator*(const RotationMatrix& lhs, const RotationMatrix& rhs);
      friend ThreeVector operator*(const RotationMatrix& m, const ThreeVector& v);
      friend FourMomentum operator*(const RotationMatrix& m, const FourMomentum& v);
  };
};
