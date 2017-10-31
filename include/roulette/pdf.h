#pragma once

#include "roulette/random_generator.h"
#include "roulette/histogram.h"
#include "roulette/distributions/probability_sampling.h"

#include "rapidjson/document.h"

namespace roulette {
  class Pdf {
    private:
      std::vector<double> m_x;
      std::vector<double> m_y;
      std::vector<double> m_cdf;
      double m_total_integral;

      distributions::ProbabilitySampling<size_t> m_index_sampling;

    public:
      Pdf();
      // Uniform
      Pdf(double x_min, double x_max);
      Pdf(const std::vector<double>& x, const std::vector<double>& y);
      Pdf(const rapidjson::Value& data);

      double min() const;
      double max() const;

      double operator()(RandomGenerator& generator) const;

    private:
      void set_index_sampling();
      void validate_values();
  };
};
