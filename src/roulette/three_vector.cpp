#include "roulette/three_vector.h"

#include <cmath>

namespace roulette {
  ThreeVector::ThreeVector(double x, double y, double z) :
    m_v(3)
  {
    m_v(0) = x;
    m_v(1) = y;
    m_v(2) = z;
  }

  ThreeVector::ThreeVector(const boost::numeric::ublas::vector<double>& v)
    : m_v(v)
  {
    assert(m_v.size() == 3);
  }

  ThreeVector::ThreeVector(boost::numeric::ublas::vector<double>&& v) : m_v(std::move(v))
  {
    assert(m_v.size() == 3);
  }

  const boost::numeric::ublas::vector<double>& ThreeVector::vector() const { return m_v; }

  double ThreeVector::operator()(int i) const {
    assert(i >= 0 && i < 3);
    return m_v(i);
  }

  double& ThreeVector::operator()(int i) {
    assert(i >= 0 && i < 3);
    return m_v(i);
  }

  double ThreeVector::x() const { return m_v(0); }
  double ThreeVector::y() const { return m_v(1); }
  double ThreeVector::z() const { return m_v(2); }

  double ThreeVector::magnitude() const {
    return std::sqrt(this->magnitude2());
  }
  double ThreeVector::magnitude2() const {
    return m_v(1)*m_v(1) + m_v(2)*m_v(2) + m_v(3)*m_v(3);
  }

  double ThreeVector::dot(const ThreeVector& other) const {
    return (
      m_v(0)*other(0) +
      m_v(1)*other(1) +
      m_v(2)*other(2)
    );
  }

  ThreeVector ThreeVector::cross(const ThreeVector& other) const {
    return {
      m_v(1)*other(2) - m_v(2)*other(1),
      m_v(2)*other(0) - m_v(0)*other(2),
      m_v(0)*other(1) - m_v(1)*other(0)
    };
  }

  bool ThreeVector::operator==(const ThreeVector& other) const {
    return (m_v(0) == other(0)) && (m_v(1) == other(1)) && (m_v(2) == other(2));
  }

  ThreeVector& ThreeVector::operator+=(const ThreeVector& rhs) {
    m_v += rhs.vector();
    return *this;
  }

  ThreeVector operator+(ThreeVector lhs, const ThreeVector& rhs) {
    lhs += rhs;
    return lhs;
  }
};

std::ostream &operator<<(std::ostream &os, const roulette::ThreeVector& v) {
  os << "ThreeVector(" << v.x() << ", " << v.y() << ", " << v.z() << ")";
  return os;
}

