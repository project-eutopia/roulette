#pragma once

#include <boost/numeric/ublas/vector.hpp>

#define SPEED_OF_LIGHT 299792458.0

namespace roulette {
  // Uses Minkowski space-time metric diag(1,-1,-1,-1)
  class FourMomentum {
    private:
      boost::numeric::ublas::vector<double> m_p;

    public:
      FourMomentum(double e_over_c = 0, double px = 0, double py = 0, double pz = 0);

      const boost::numeric::ublas::vector<double>& vector() const;

      double e_over_c() const;
      double px() const;
      double py() const;
      double pz() const;

      double mass() const;
      double mass2() const;
      double energy() const;
      double momentum_magnitude() const;
      double momentum_magnitude2() const;

      bool operator==(const FourMomentum& other) const;
      FourMomentum& operator+=(const FourMomentum& rhs);
      friend FourMomentum operator+(FourMomentum lhs, const FourMomentum& rhs);
  };
};

std::ostream &operator<<(std::ostream &os, const roulette::FourMomentum& p);

