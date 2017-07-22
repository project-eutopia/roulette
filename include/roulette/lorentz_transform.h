#pragma once

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>

#include "roulette/three_vector.h"
#include "roulette/four_momentum.h"

namespace roulette {
  class LorentzTransform {
    private:
      boost::numeric::ublas::matrix<double> m_matrix;

    public:
      LorentzTransform();
      LorentzTransform(const boost::numeric::ublas::matrix<double>& matrix);
      LorentzTransform(boost::numeric::ublas::matrix<double>&& matrix);
      LorentzTransform(
        double m00, double m01, double m02, double m03,
        double m10, double m11, double m12, double m13,
        double m20, double m21, double m22, double m23,
        double m30, double m31, double m32, double m33
      );

      double operator()(int i, int j) const;

      const boost::numeric::ublas::matrix<double>& matrix() const;
      static LorentzTransform rotationX(double angle);
      static LorentzTransform rotationY(double angle);
      static LorentzTransform rotationZ(double angle);
      static LorentzTransform rotationU(double angle, const ThreeVector& u);
      static LorentzTransform rotationUtoV(const ThreeVector& u, const ThreeVector& v);

      friend LorentzTransform operator*(const LorentzTransform& lhs, const LorentzTransform& rhs);
      friend FourMomentum operator*(const LorentzTransform& m, const FourMomentum& v);
  };
};
