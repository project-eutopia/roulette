#pragma once

#include <boost/numeric/ublas/vector.hpp>
#include "roulette/three_vector.h"

namespace roulette {
  // Uses Minkowski space-time metric diag(1,-1,-1,-1)
  class FourMomentum {
    private:
      boost::numeric::ublas::vector<double> m_p;

    public:
      FourMomentum(double e = 0, double px = 0, double py = 0, double pz = 0);
      FourMomentum(const boost::numeric::ublas::vector<double>& p);
      FourMomentum(boost::numeric::ublas::vector<double>&& p);
      FourMomentum(double energy, double mass, const ThreeVector& direction);

      const boost::numeric::ublas::vector<double>& vector() const;
      boost::numeric::ublas::vector<double>& vector();

      double operator()(int i) const;
      double& operator()(int i);

      double e() const;
      double px() const;
      double py() const;
      double pz() const;

      double mass() const;
      double mass2() const;
      double energy() const;
      double momentum_magnitude() const;
      double momentum_magnitude2() const;

      double dot(const FourMomentum& other) const;

      ThreeVector direction_unit_vector() const;

      bool operator==(const FourMomentum& other) const;

      FourMomentum& operator+=(const FourMomentum& rhs);
      friend FourMomentum operator+(const FourMomentum& lhs, const FourMomentum& rhs);
      friend FourMomentum operator-(const FourMomentum& lhs, const FourMomentum& rhs);
      friend FourMomentum operator*(double lhs, const FourMomentum& rhs);
      friend FourMomentum operator*(const FourMomentum& lhs, double rhs);
      friend FourMomentum operator/(double lhs, const FourMomentum& rhs);
      friend FourMomentum operator/(const FourMomentum& lhs, double rhs);
  };
};

std::ostream &operator<<(std::ostream &os, const roulette::FourMomentum& p);

