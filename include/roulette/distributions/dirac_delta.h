#pragma once

#include "roulette/random_generator.h"
#include "roulette/distribution.h"
#include "roulette/histogram.h"

namespace roulette {
  namespace distributions {
    class DiracDelta : public Distribution {
      private:
        double m_x;

      public:
        DiracDelta(double x);
        double operator()(RandomGenerator& generator);
        double area_between(double x0, double x1) const;
        void fill_in_expected_histogram(Histogram& expected, int num_samples = 1) const;

        double x() const;
        void set_x(double x);
    };
  };
};
