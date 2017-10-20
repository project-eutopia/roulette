#pragma once

#include <memory>
#include "roulette/three_vector.h"
#include "roulette/geometries/geometry.h"

namespace roulette {
  namespace geometries {
    class Ellipse : public Geometry {
      private:
        const ThreeVector m_center;
        // Unit vector in "x" direction (that with radius m_a)
        const ThreeVector m_u1;
        // Unit vector in "y" direction (that with radius m_b)
        const ThreeVector m_u2;

        const double m_a;
        const double m_b;

      public:
        static std::shared_ptr<Ellipse> from_json(const rapidjson::Value& data);
        Ellipse(const ThreeVector& center, const ThreeVector& vertex, const ThreeVector& covertex);

        ThreeVector sample(RandomGenerator& generator) const;

        double radius(double theta) const;

      private:
        double random_theta(RandomGenerator& generator) const;
    };
  };
};
