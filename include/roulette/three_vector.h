#pragma once

#include <boost/numeric/ublas/vector.hpp>

namespace roulette {
  class ThreeVector {
    private:
      boost::numeric::ublas::vector<double> m_v;

    public:
      ThreeVector(double x = 0, double y = 0, double z = 0);
      ThreeVector(const boost::numeric::ublas::vector<double>& v);
      ThreeVector(boost::numeric::ublas::vector<double>&& v);

      const boost::numeric::ublas::vector<double>& vector() const;

      double operator()(int i) const;
      double& operator()(int i);

      double x() const;
      double y() const;
      double z() const;

      double magnitude() const;
      double magnitude2() const;

      double dot(const ThreeVector& other) const;
      ThreeVector cross(const ThreeVector& other) const;

      bool operator==(const ThreeVector& other) const;

      friend ThreeVector operator+(const ThreeVector& lhs, const ThreeVector& rhs);
      friend ThreeVector operator-(const ThreeVector& lhs, const ThreeVector& rhs);
      friend ThreeVector operator*(const ThreeVector& lhs, double rhs);
      friend ThreeVector operator*(double lhs, const ThreeVector& rhs);
      friend ThreeVector operator/(const ThreeVector& lhs, double rhs);
      friend ThreeVector operator/(double lhs, const ThreeVector& rhs);
  };
};

std::ostream &operator<<(std::ostream &os, const roulette::ThreeVector& p);

