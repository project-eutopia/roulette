#include "roulette/rotation_matrix.h"

#include <cmath>

namespace roulette {
  RotationMatrix::RotationMatrix()
    : m_matrix()
  {
    m_matrix[0][0] = 1;
    m_matrix[0][1] = 0;
    m_matrix[0][2] = 0;

    m_matrix[1][0] = 0;
    m_matrix[1][1] = 1;
    m_matrix[1][2] = 0;

    m_matrix[2][0] = 0;
    m_matrix[2][1] = 0;
    m_matrix[2][2] = 1;
  }

  RotationMatrix::RotationMatrix(const std::array<std::array<double,3>,3>& matrix) :
    m_matrix(matrix)
  {
  }

  RotationMatrix::RotationMatrix(
    double m00, double m01, double m02,
    double m10, double m11, double m12,
    double m20, double m21, double m22
  )
  {
    m_matrix[0][0] = m00;
    m_matrix[0][1] = m01;
    m_matrix[0][2] = m02;

    m_matrix[1][0] = m10;
    m_matrix[1][1] = m11;
    m_matrix[1][2] = m12;

    m_matrix[2][0] = m20;
    m_matrix[2][1] = m21;
    m_matrix[2][2] = m22;
  }

  double RotationMatrix::operator()(int i, int j) const {
    assert(i >= 0 && i < 3);
    assert(j >= 0 && j < 3);
    return m_matrix[i][j];
  }

  RotationMatrix RotationMatrix::rotationX(double angle) {
    double sin = std::sin(angle);
    double cos = std::cos(angle);

    return RotationMatrix(
      1,   0,    0,
      0, cos, -sin,
      0, sin,  cos
    );
  }

  RotationMatrix RotationMatrix::rotationY(double angle) {
    double sin = std::sin(angle);
    double cos = std::cos(angle);

    return RotationMatrix(
       cos,   0,  sin,
         0,   1,    0,
      -sin,   0,  cos
    );
  }

  RotationMatrix RotationMatrix::rotationZ(double angle) {
    double sin = std::sin(angle);
    double cos = std::cos(angle);

    return RotationMatrix(
      cos,-sin,    0,
      sin, cos,    0,
        0,   0,    1
    );
  }

  RotationMatrix RotationMatrix::rotationU(double angle, const ThreeVector& u) {
    double s = std::sin(angle);
    double c = std::cos(angle);

    return RotationMatrix(
      c + u(0)*u(0)*(1-c), u(1)*u(2)*(1-c) - u(2)*s, u(0)*u(2)*(1-c) + u(1)*s,
      u(1)*u(2)*(1-c) + u(2)*s, c + u(1)*u(1)*(1-c), u(1)*u(2)*(1-c) - u(0)*s,
      u(0)*u(2)*(1-c) - u(1)*s, u(1)*u(2)*(1-c) + u(0)*s, c + u(2)*u(2)*(1-c)
    );
  }

  RotationMatrix RotationMatrix::rotationUtoV(const ThreeVector& u, const ThreeVector& v) {
    ThreeVector cross = u.cross(v);

    double cos = u.dot(v);
    // Degenerate case impossible -- u and v opposite
    assert(cos > -1);
    double C = 1 / (1+cos);

    return RotationMatrix(
      1 - C*(cross(2)*cross(2) + cross(1)*cross(1)), -cross(2) + C*cross(0)*cross(1), cross(1) + C*cross(0)*cross(2),
      cross(2) + C*cross(0)*cross(1), 1 - C*(cross(0)*cross(0) + cross(2)*cross(2)), -cross(0) + C*cross(1)*cross(2),
      -cross(1) + C*cross(0)*cross(2), cross(0) + C*cross(1)*cross(2), 1 - C*(cross(0)*cross(0) + cross(1)*cross(1))
    );
  }

  RotationMatrix RotationMatrix::inv() const {
    // Orthogonal, so just transpose
    return RotationMatrix(
      m_matrix[0][0], m_matrix[1][0], m_matrix[2][0],
      m_matrix[0][1], m_matrix[1][1], m_matrix[2][1],
      m_matrix[0][2], m_matrix[1][2], m_matrix[2][2]
    );
  }

  RotationMatrix operator*(const RotationMatrix& lhs, const RotationMatrix& rhs) {
    std::array<std::array<double,3>,3> matrix;

    int i, j;
    for (i = 0; i < 3; ++i) {
      for (j = 0; j < 3; ++j) {
        matrix[i][j] = lhs(i,0)*rhs(0,j) + lhs(i,1)*rhs(1,j) + lhs(i,2)*rhs(2,j);
      }
    }
    return RotationMatrix(matrix);
  }

  ThreeVector operator*(const RotationMatrix& m, const ThreeVector& v) {
    return ThreeVector(
      m(0,0)*v(0) + m(0,1)*v(1) + m(0,2)*v(2),
      m(1,0)*v(0) + m(1,1)*v(1) + m(1,2)*v(2),
      m(2,0)*v(0) + m(2,1)*v(1) + m(2,2)*v(2)
    );
  }

  FourMomentum operator*(const RotationMatrix& m, const FourMomentum& v) {
    return FourMomentum(
      v(0),
      m(0,0)*v(1) + m(0,1)*v(2) + m(0,2)*v(3),
      m(1,0)*v(1) + m(1,1)*v(2) + m(1,2)*v(3),
      m(2,0)*v(1) + m(2,1)*v(2) + m(2,2)*v(3)
    );
  }
};
