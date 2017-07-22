#pragma once

#include <vector>
#include <ostream>
#include <functional>

namespace roulette {
  class Histogram {
    private:
      double m_low;
      double m_high;
      double m_delta;
      int m_nbins;
      double m_total;
      // Stores nbins + 2 (for before and after bins)
      std::vector<double> m_counts;

    public:
      Histogram(double low, double high, int nbins);

      int nbins() const;
      double low() const;
      double high() const;
      double total() const;

      double bin_low_x(int i) const;
      double bin_high_x(int i) const;

      bool compatible_with(const Histogram& other) const;

      void add_point(double x);

      // Used for setting up expected histograms
      void set_bin(int i, double p);
      void set_total(double total);

      // < 0 for before low, >= nbins for after high
      // Normalized to have total of 1.0
      double bin_at(int i) const;
      double count_at(int i) const;

      int number_of_nonzero_bins() const;

      void write_json(std::ostream& stream) const;

      void fill_in_from_cdf(std::function<double(double,double)> cdf, int num_samples = 1);

    private:
      int count_bin(int public_index) const;
  };
};
