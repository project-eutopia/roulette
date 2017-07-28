#include "roulette/non_uniform_linear_interpolation.h"

#include <cassert>
#include <fstream>
#include <sstream>
#include <string>

#include "roulette/math.h"

namespace roulette {
  NonUniformLinearInterpolation::NonUniformLinearInterpolation() :
    m_x(),
    m_y()
  {}

  NonUniformLinearInterpolation::NonUniformLinearInterpolation(const std::vector<double>& x, const std::vector<double>& y) :
    m_x(x),
    m_y(y)
  {
    assert(m_x.size() == m_y.size());
  }

  NonUniformLinearInterpolation::NonUniformLinearInterpolation(std::vector<double>&& x, std::vector<double>&& y) :
    m_x(x),
    m_y(y)
  {
    assert(m_x.size() == m_y.size());
  }

  NonUniformLinearInterpolation::NonUniformLinearInterpolation(const std::string& filename) :
    m_x(0),
    m_y(0)
  {
    std::ifstream infile(filename);

    std::string line;
    while (std::getline(infile, line)) {
      std::istringstream iss(line);
      double x, y;
      // Skip line if cannot process
      if (!(iss >> x >> y)) { continue; }

      this->add_point(x, y);
    }
  }

  void NonUniformLinearInterpolation::reserve(int n) {
    m_x.reserve(n);
    m_y.reserve(n);
  }

  void NonUniformLinearInterpolation::add_point(double x, double y) {
    if (m_x.size() > 0) {
      assert(m_x.back() < x);
    }
    m_x.push_back(x);
    m_y.push_back(y);
  }

  const std::vector<double>& NonUniformLinearInterpolation::xs() const { return m_x; }
  const std::vector<double>& NonUniformLinearInterpolation::ys() const { return m_y; }

  double NonUniformLinearInterpolation::operator()(double x) const {
    assert(m_x.size() > 0);

    if (x < m_x.front()) return m_y.front();
    if (x >= m_x.back()) return m_y.back();

    if (m_x.size() == 1) return m_y[0];

    int l, r, m;
    l = 0;
    r = m_x.size()-2;

    m = floori( (l+r)/2.0 );

    // Binary search for MLC
    while (l < r) {
      if (x < m_x[m]) {
        r = m-1;
      }
      else if (x >= m_x[m+1]) {
        l = m+1;
      } else {
        // HIT
        break;
      }

      m = floori( (l+r)/2.0 );
    }

    return m_y[m] + (x - m_x[m]) * (m_y[m+1] - m_y[m]) / (m_x[m+1] - m_x[m]);
  }

  NonUniformLinearInterpolation NonUniformLinearInterpolation::linear_combination(const std::vector<const NonUniformLinearInterpolation*>& interpolations, const std::vector<double>& weights) {
    assert(interpolations[0].size() > 0);

    std::vector<double> xs;
    std::vector<double> ys;

    xs.reserve(interpolations[0]->xs().size());
    ys.reserve(interpolations[0]->ys().size());

    double y;
    int i, w;
    for (i = 0; i < interpolations[0]->xs().size(); ++i) {
      xs.push_back(interpolations[0]->xs()[i]);

      y = 0;
      for (w = 0; w < interpolations.size(); ++w) {
        y += weights[w] * (*interpolations[w])(xs.back());
      }
      ys.push_back(y);
    }

    return NonUniformLinearInterpolation(xs, ys);
  }
};
