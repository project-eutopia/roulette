#include "roulette/four_momentum.h"

#include <cmath>

namespace roulette {
  FourMomentum::FourMomentum(double e, double px, double py, double pz) :
    m_p(4)
  {
    m_p(0) = e;
    m_p(1) = px;
    m_p(2) = py;
    m_p(3) = pz;
  }

  FourMomentum::FourMomentum(const boost::numeric::ublas::vector<double>& p)
    : m_p(p)
  {
    assert(m_p.size() == 4);
  }

  FourMomentum::FourMomentum(boost::numeric::ublas::vector<double>&& p) : m_p(std::move(p))
  {
    assert(m_p.size() == 4);
  }

  FourMomentum::FourMomentum(double energy, double mass, const ThreeVector& direction) :
    m_p(energy)
  {
    // e^2 - p^2 = m^2
    // |p| = sqrt(e^2 - m^2)
    // So multiple direction vector by |p| / |direction|
    double factor = std::sqrt(energy*energy - mass*mass) / direction.magnitude();

    m_p(1) = factor * direction(0);
    m_p(2) = factor * direction(1);
    m_p(3) = factor * direction(2);
  }

  const boost::numeric::ublas::vector<double>& FourMomentum::vector() const { return m_p; }
  boost::numeric::ublas::vector<double>& FourMomentum::vector() { return m_p; }

  double FourMomentum::operator()(int i) const {
    assert(i >= 0 && i < 4);
    return m_p(i);
  }

  double& FourMomentum::operator()(int i) {
    assert(i >= 0 && i < 4);
    return m_p(i);
  }

  double FourMomentum::e() const { return m_p(0); }
  double FourMomentum::px() const { return m_p(1); }
  double FourMomentum::py() const { return m_p(2); }
  double FourMomentum::pz() const { return m_p(3); }

  double FourMomentum::mass() const {
    return std::sqrt(this->mass2());
  }
  double FourMomentum::mass2() const {
    return this->dot(*this);
  }

  double FourMomentum::energy() const {
    return m_p(0);
  }

  double FourMomentum::momentum_magnitude() const {
    return std::sqrt(this->momentum_magnitude2());
  }
  double FourMomentum::momentum_magnitude2() const {
    return m_p(1)*m_p(1) + m_p(2)*m_p(2) + m_p(3)*m_p(3);
  }

  double FourMomentum::dot(const FourMomentum& other) const {
    return m_p(0)*other(0) - m_p(1)*other(1) - m_p(2)*other(2) - m_p(3)*other(3);
  }

  ThreeVector FourMomentum::direction_unit_vector() const {
    double f = this->momentum_magnitude();
    return ThreeVector(
      this->px() / f,
      this->py() / f,
      this->pz() / f
    );
  }

  bool FourMomentum::operator==(const FourMomentum& other) const {
    return (m_p(0) == other.e())
      && (m_p(1) == other.px())
      && (m_p(2) == other.py())
      && (m_p(3) == other.pz());
  }

  FourMomentum operator+(const FourMomentum& lhs, const FourMomentum& rhs) {
    return FourMomentum(lhs.m_p + rhs.m_p);
  }

  FourMomentum operator-(const FourMomentum& lhs, const FourMomentum& rhs) {
    return FourMomentum(lhs.m_p - rhs.m_p);
  }

  FourMomentum operator*(const FourMomentum& lhs, double rhs) {
    return FourMomentum(lhs.m_p * rhs);
  }

  FourMomentum operator*(double lhs, const FourMomentum& rhs) {
    return FourMomentum(lhs * rhs.m_p);
  }

  FourMomentum operator/(const FourMomentum& lhs, double rhs) {
    return FourMomentum(lhs.m_p / rhs);
  }

  FourMomentum operator/(double lhs, const FourMomentum& rhs) {
    return FourMomentum(lhs / rhs.m_p);
  }
};

std::ostream &operator<<(std::ostream &os, const roulette::FourMomentum& p) {
  os << "FourMomentum(" << p.e() << ", " << p.px() << ", " << p.py() << ", " << p.pz() << ")";
  return os;
}

