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

  FourMomentum::FourMomentum(const FourMomentum& momentum) :
    FourMomentum(momentum.m_p)
  {}

  FourMomentum::FourMomentum(FourMomentum&& momentum) :
    FourMomentum(momentum.m_p)
  {}

  const boost::numeric::ublas::vector<double>& FourMomentum::vector() const { return m_p; }

  double FourMomentum::operator()(int i) const {
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
    return m_p(0)*m_p(0) - this->momentum_magnitude2();
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

  FourMomentum& FourMomentum::operator+=(const FourMomentum& rhs) {
    m_p += rhs.vector();
    return *this;
  }

  FourMomentum operator+(FourMomentum lhs, const FourMomentum& rhs) {
    lhs += rhs;
    return lhs;
  }
};

std::ostream &operator<<(std::ostream &os, const roulette::FourMomentum& p) {
  os << "FourMomentum(" << p.e() << ", " << p.px() << ", " << p.py() << ", " << p.pz() << ")";
  return os;
}

