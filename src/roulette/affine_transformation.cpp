#include "roulette/affine_transformation.h"

#include <cmath>

namespace roulette {
  AffineTransformation::AffineTransformation()
    : m_rotation(),
      m_translation()
  {
  }

  AffineTransformation::AffineTransformation(const RotationMatrix& rotation)
    : m_rotation(rotation),
      m_translation()
  {
  }

  AffineTransformation::AffineTransformation(const ThreeVector& translation)
    : m_rotation(),
      m_translation(translation)
  {
  }

  AffineTransformation::AffineTransformation(const RotationMatrix& rotation, const ThreeVector& translation)
    : m_rotation(rotation),
      m_translation(translation)
  {
  }

  const RotationMatrix& AffineTransformation::rotation_part() const { return m_rotation; }
  const ThreeVector& AffineTransformation::translation_part() const { return m_translation; }

  AffineTransformation AffineTransformation::inv() const {
    return AffineTransformation(
      m_rotation.inv(),
      -(m_rotation.inv()*m_translation)
    );
  }

  AffineTransformation operator*(const AffineTransformation& lhs, const AffineTransformation& rhs) {
    return AffineTransformation(
      lhs.rotation_part() * rhs.rotation_part(),
      lhs.rotation_part() * rhs.translation_part() + lhs.translation_part()
    );
  }

  AffineTransformation operator*(const RotationMatrix& lhs, const AffineTransformation& rhs) {
    return AffineTransformation(
      lhs * rhs.rotation_part(),
      lhs * rhs.translation_part()
    );
  }

  AffineTransformation operator*(const AffineTransformation& lhs, const RotationMatrix& rhs) {
    return AffineTransformation(
      lhs.rotation_part() * rhs,
      lhs.translation_part()
    );
  }

  ThreeVector AffineTransformation::transform_point(const ThreeVector& point) const {
    return m_rotation * point + m_translation;
  }

  ThreeVector AffineTransformation::transform_vector(const ThreeVector& vector) const {
    return m_rotation * vector;
  }
};
