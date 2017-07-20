#pragma once

#include <vector>
#include <ostream>

namespace roulette {
  class Histogram {
    private:
      // Stores nbins + 2 (for before and after bins)
      std::vector<double> m_counts;
      double m_low;
      double m_high;
      double m_delta;
      int m_nbins;
      int m_total;

    public:
      Histogram(double low, double high, int nbins);

      int nbins() const;
      double low() const;
      double high() const;

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

      double chi_square_vs_expected(const Histogram& expected) const;

      void write_json(std::ostream& stream) const;

    private:
      int count_bin(int public_index) const;
  };
};
