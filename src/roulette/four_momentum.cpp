#include "roulette/four_momentum.h"

#include <cmath>

namespace roulette {
  FourMomentum::FourMomentum(double e_over_c, double px, double py, double pz) :
    m_p(4)
  {
    m_p(0) = e_over_c;
    m_p(1) = px;
    m_p(2) = py;
    m_p(3) = pz;
  }

  const boost::numeric::ublas::vector<double>& FourMomentum::p() const { return m_p; }

  double FourMomentum::e_over_c() const { return m_p(0); }
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
    return m_p(0) * SPEED_OF_LIGHT;
  }

  double FourMomentum::momentum_magnitude() const {
    return std::sqrt(this->momentum_magnitude2());
  }
  double FourMomentum::momentum_magnitude2() const {
    return m_p(1)*m_p(1) + m_p(2)*m_p(2) + m_p(3)*m_p(3);
  }

  bool FourMomentum::operator==(const FourMomentum& other) const {
    return (m_p(0) == other.e_over_c())
      && (m_p(1) == other.px())
      && (m_p(2) == other.py())
      && (m_p(3) == other.pz());
  }

  FourMomentum& FourMomentum::operator+=(const FourMomentum& rhs) {
    m_p += rhs.p();
    return *this;
  }

  FourMomentum operator+(FourMomentum lhs, const FourMomentum& rhs) {
    lhs += rhs;
    return lhs;
  }
};

std::ostream &operator<<(std::ostream &os, const roulette::FourMomentum& p) {
  os << "FourMomentum(" << p.e_over_c() << ", " << p.px() << ", " << p.py() << ", " << p.pz() << ")";
  return os;
}

