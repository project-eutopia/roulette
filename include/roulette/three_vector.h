#pragma once

#include <boost/numeric/ublas/vector.hpp>
#include <fstream>

#include "rapidjson/document.h"

namespace roulette {
  class ThreeVector {
    private:
      boost::numeric::ublas::vector<double> m_v;

    public:
      ThreeVector(const rapidjson::Value& data);
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

      ThreeVector direction_unit_vector() const;

      double dot(const ThreeVector& other) const;
      ThreeVector cross(const ThreeVector& other) const;

      bool operator==(const ThreeVector& other) const;

      ThreeVector& operator+=(const ThreeVector& rhs);
      friend ThreeVector operator+(const ThreeVector& lhs, const ThreeVector& rhs);
      friend ThreeVector operator-(const ThreeVector& lhs, const ThreeVector& rhs);
      friend ThreeVector operator*(const ThreeVector& lhs, double rhs);
      friend ThreeVector operator*(double lhs, const ThreeVector& rhs);
      friend ThreeVector operator/(const ThreeVector& lhs, double rhs);

      std::ofstream& write(std::ofstream& os) const;
      std::ifstream& read(std::ifstream& is);

      rapidjson::Value to_json(rapidjson::Document::AllocatorType& allocator) const;
  };
};

std::ostream &operator<<(std::ostream &os, const roulette::ThreeVector& p);
