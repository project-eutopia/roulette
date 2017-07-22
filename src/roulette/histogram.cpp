#include "roulette/histogram.h"

#include <cassert>
#include <string>
#include <limits>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace roulette {
  Histogram::Histogram(double low, double high, int nbins) :
    m_low(low),
    m_high(high),
    m_delta((high-low) / nbins),
    m_nbins(nbins),
    m_total(0),
    m_counts(nbins+2, 0)
  {
  }

  int Histogram::nbins() const { return m_nbins; }
  double Histogram::low() const { return m_low; }
  double Histogram::high() const { return m_high; }
  double Histogram::total() const { return m_total; }

  double Histogram::bin_low_x(int i) const {
    if (i < 0) {
      return -std::numeric_limits<double>::infinity();
    }
    else if (i >= m_nbins) {
      return m_high;
    }
    else {
      return m_low + i * m_delta;
    }
  }

  double Histogram::bin_high_x(int i) const {
    if (i < 0) {
      return m_low;
    }
    else if (i >= m_nbins) {
      return std::numeric_limits<double>::infinity();
    }
    else {
      return m_low + (i+1) * m_delta;
    }
  }

  bool Histogram::compatible_with(const Histogram& other) const {
    return (this->nbins() == other.nbins()) &&
      (this->low() == other.low()) &&
      (this->high() == other.high());
  }

  void Histogram::add_point(double x) {
    int i = (int)((x - m_low) / m_delta);
    ++m_counts[this->count_bin(i)];
    ++m_total;
  }

  void Histogram::set_bin(int i, double p) {
    m_counts[this->count_bin(i)] = p;
  }

  void Histogram::set_total(double total) {
    m_total = total;
  }

  double Histogram::bin_at(int i) const {
    return m_counts[this->count_bin(i)] / m_total;
  }

  double Histogram::count_at(int i) const {
    return m_counts[this->count_bin(i)];
  }

  int Histogram::number_of_nonzero_bins() const {
    int num = 0;
    for (int i = -1; i <= this->nbins(); ++i) {
      if (this->count_at(i) > 0) ++num;
    }
    return num;
  }

  int Histogram::count_bin(int public_index) const {
    if (public_index < 0) {
      return 0;
    }
    else if (public_index >= m_nbins) {
      return m_nbins+1;
    }
    else {
      return public_index+1;
    }
  }

  void Histogram::fill_in_from_cdf(std::function<double(double,double)> cdf, int num_samples) {
    for (int i = -1; i <= m_nbins; ++i) {
      this->set_bin(i, num_samples * cdf(this->bin_low_x(i), this->bin_high_x(i)));
    }

    this->set_total(num_samples);
  }

  void Histogram::write_json(std::ostream& stream) const {
    boost::property_tree::ptree root;

    boost::property_tree::ptree bins;

    for (int i = -1; i <= m_nbins; ++i) {
      boost::property_tree::ptree bin;
      bin.put("i", i);
      bin.put("low", this->bin_low_x(i));
      bin.put("high", this->bin_high_x(i));
      bin.put("bin", this->bin_at(i));
      bin.put("count", this->count_at(i));

      bins.push_back(std::make_pair(std::to_string(i), bin));
    }

    root.put("nbins", this->nbins());
    root.put("total", this->total());
    root.put("low", this->low());
    root.put("high", this->high());
    root.push_back(std::make_pair("bins", bins));

    boost::property_tree::write_json(stream, root);
  }
};
