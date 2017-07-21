#pragma once

#include "roulette/random_generator.h"
#include "roulette/distribution.h"
#include "roulette/histogram.h"

namespace roulette {
  namespace distributions {
    class Uniform : public Distribution {
      private:
        double m_a;
        double m_b;

      public:
        Uniform(double a = 0, double b = 1);
        double operator()(RandomGenerator& generator);
        double area_between(double x0, double x1) const;
        void fill_in_expected_histogram(Histogram& expected, int num_samples = 1) const;

        double a() const;
        double b() const;

        void set_a(double a);
        void set_b(double b);
    };
  };
};
