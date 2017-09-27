#pragma once

#include "roulette/three_vector.h"
#include "roulette/rotation_matrix.h"

// NOTE:  We are limiting ourselves to orthogonal affine transformations only --
// those which are a combination of translations and rotations
namespace roulette {
  class AffineTransformation {
    private:
      RotationMatrix m_rotation;
      ThreeVector m_translation;

    public:
      // Identity
      AffineTransformation();
      AffineTransformation(const RotationMatrix& rotation);
      AffineTransformation(const ThreeVector& translation);
      AffineTransformation(const RotationMatrix& rotation, const ThreeVector& translation);

      const RotationMatrix& rotation_part() const;
      const ThreeVector& translation_part() const;

      AffineTransformation inv() const;

      friend AffineTransformation operator*(const AffineTransformation& lhs, const AffineTransformation& rhs);
      friend AffineTransformation operator*(const RotationMatrix& lhs, const AffineTransformation& rhs);
      friend AffineTransformation operator*(const AffineTransformation& lhs, const RotationMatrix& rhs);

      ThreeVector transform_point(const ThreeVector& point) const;
      ThreeVector transform_vector(const ThreeVector& vector) const;
  };
};
