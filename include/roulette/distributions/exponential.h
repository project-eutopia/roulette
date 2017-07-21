#pragma once

#include "roulette/histogram.h"
#include "roulette/real_function.h"
#include "roulette/random_generator.h"
#include "roulette/distribution.h"

namespace roulette {
  namespace distributions {
    // PDF:  mu exp(-mu * x) for x in [0,inf)
    // mu must be positive
    class Exponential : public Distribution {
      private:
        double m_mu;

      public:
        Exponential(double mu);
        double operator()(RandomGenerator& generator);
        double area_between(double x0, double x1) const;
        void fill_in_expected_histogram(Histogram& expected, int num_samples = 1) const;

        double mu() const;
        void set_mu(double mu);
    };
  };
};
