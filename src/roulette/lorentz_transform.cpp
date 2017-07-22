#include "roulette/lorentz_transform.h"

#include <cmath>

namespace roulette {
  LorentzTransform::LorentzTransform() : m_matrix(boost::numeric::ublas::identity_matrix<double>(4))
  {
  }

  LorentzTransform::LorentzTransform(const boost::numeric::ublas::matrix<double>& matrix) : m_matrix(matrix)
  {
    assert(m_matrix.size1() == 4);
    assert(m_matrix.size2() == 4);
  }

  LorentzTransform::LorentzTransform(boost::numeric::ublas::matrix<double>&& matrix) :
    m_matrix(std::move(matrix))
  {
    assert(m_matrix.size1() == 4);
    assert(m_matrix.size2() == 4);
  }

  LorentzTransform::LorentzTransform(
    double m00, double m01, double m02, double m03,
    double m10, double m11, double m12, double m13,
    double m20, double m21, double m22, double m23,
    double m30, double m31, double m32, double m33
  )
    : m_matrix(4,4)
  {
    m_matrix(0,0) = m00;
    m_matrix(0,1) = m01;
    m_matrix(0,2) = m02;
    m_matrix(0,3) = m03;

    m_matrix(1,0) = m10;
    m_matrix(1,1) = m11;
    m_matrix(1,2) = m12;
    m_matrix(1,3) = m13;

    m_matrix(2,0) = m20;
    m_matrix(2,1) = m21;
    m_matrix(2,2) = m22;
    m_matrix(2,3) = m23;

    m_matrix(3,0) = m30;
    m_matrix(3,1) = m31;
    m_matrix(3,2) = m32;
    m_matrix(3,3) = m33;
  }

  const boost::numeric::ublas::matrix<double>& LorentzTransform::matrix() const {
    return m_matrix;
  }

  double LorentzTransform::operator()(int i, int j) const {
    assert(i >= 0 && i < 4);
    assert(j >= 0 && j < 4);
    return m_matrix(i,j);
  }
  LorentzTransform LorentzTransform::rotationX(double angle) {
    double sin = std::sin(angle);
    double cos = std::cos(angle);

    return LorentzTransform(
      1,   0,   0,    0,
      0,   1,   0,    0,
      0,   0, cos, -sin,
      0,   0, sin,  cos
    );
  }

  LorentzTransform LorentzTransform::rotationY(double angle) {
    double sin = std::sin(angle);
    double cos = std::cos(angle);

    return LorentzTransform(
      1,   0,   0,    0,
      0, cos,   0,  sin,
      0,   0,   1,    0,
      0,-sin,   0,  cos
    );
  }

  LorentzTransform LorentzTransform::rotationZ(double angle) {
    double sin = std::sin(angle);
    double cos = std::cos(angle);

    return LorentzTransform(
      1,   0,   0,    0,
      0, cos,-sin,    0,
      0, sin, cos,    0,
      0,   0,   0,    1
    );
  }

  LorentzTransform LorentzTransform::rotationU(double angle, const ThreeVector& u) {
    double s = std::sin(angle);
    double c = std::cos(angle);

    return LorentzTransform(
      1, 0, 0, 0,
      0, c + u(0)*u(0)*(1-c), u(1)*u(2)*(1-c) - u(2)*s, u(0)*u(2)*(1-c) + u(1)*s,
      0, u(1)*u(2)*(1-c) + u(2)*s, c + u(1)*u(1)*(1-c), u(1)*u(2)*(1-c) - u(0)*s,
      0, u(0)*u(2)*(1-c) - u(1)*s, u(1)*u(2)*(1-c) + u(0)*s, c + u(2)*u(2)*(1-c)
    );
  }

  LorentzTransform LorentzTransform::rotationUtoV(const ThreeVector& u, const ThreeVector& v) {
    ThreeVector cross = u.cross(v);

    double cos = u.dot(v);
    // Degenerate case impossible -- u and v opposite
    assert(cos > -1);
    double C = 1 / (1+cos);

    return LorentzTransform(
      1, 0, 0, 0,
      0, 1 - C*(cross(2)*cross(2) + cross(1)*cross(1)), -cross(2) + C*cross(0)*cross(1), cross(1) + C*cross(0)*cross(2),
      0, cross(2) + C*cross(0)*cross(1), 1 - C*(cross(0)*cross(0) + cross(2)*cross(2)), -cross(0) + C*cross(1)*cross(2),
      0, -cross(1) + C*cross(0)*cross(2), cross(0) + C*cross(1)*cross(2), 1 - C*(cross(0)*cross(0) + cross(1)*cross(1))
    );
  }

  LorentzTransform operator*(const LorentzTransform& lhs, const LorentzTransform& rhs) {
    return LorentzTransform(
      boost::numeric::ublas::prod(lhs.matrix(), rhs.matrix())
    );
  }

  FourMomentum operator*(const LorentzTransform& m, const FourMomentum& v) {
    return FourMomentum(
      boost::numeric::ublas::prod(m.matrix(), v.vector())
    );
  }
};
