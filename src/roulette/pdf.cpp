#include "roulette/pdf.h"

#include <vector>

namespace roulette {
  Pdf::Pdf() {}

  Pdf::Pdf(double x_min, double x_max) {
    m_x.reserve(2);
    m_x.push_back(x_min);
    m_x.push_back(x_max);

    m_y.reserve(2);
    m_y.push_back(1.0 / (x_max - x_min));
    m_y.push_back(1.0 / (x_max - x_min));

    this->validate_values();
    this->set_index_sampling();
  }

  Pdf::Pdf(const std::vector<double>& x, const std::vector<double>& y) :
    m_x(x), m_y(y)
  {
    this->validate_values();
    this->set_index_sampling();
  }

  Pdf::Pdf(const rapidjson::Value& data) {
    if (!data.HasMember("x") || !data["x"].IsArray()) {
      throw std::runtime_error("NonUniformLinearInterpolation JSON must have \"x\"");
    }

    if (!data.HasMember("y") || !data["y"].IsArray()) {
      throw std::runtime_error("NonUniformLinearInterpolation JSON must have \"y\"");
    }

    m_x.reserve(data["x"].Size());
    m_y.reserve(data["y"].Size());

    for (size_t i = 0; i < data["x"].Size(); ++i) {
      if (!data["x"][i].IsNumber()) {
        throw std::runtime_error("Pdf x values must be numbers");
      }
      m_x.push_back(data["x"][i].GetDouble());
    }

    for (size_t i = 0; i < data["y"].Size(); ++i) {
      if (!data["y"][i].IsNumber()) {
        throw std::runtime_error("Pdf y values must be numbers");
      }
      m_y.push_back(data["y"][i].GetDouble());
    }

    this->validate_values();
    this->set_index_sampling();
  }

  double Pdf::min() const { return m_x.front(); }
  double Pdf::max() const { return m_x.back(); }

  // See: https://stats.stackexchange.com/a/281543
  double Pdf::operator()(RandomGenerator& generator) const {
    size_t i = m_index_sampling(generator);

    double u;
    if (m_y[i] == m_y[i+1]) {
      u = generator.uniform();
    }
    else {
      double h = 2*m_y[i] / (m_y[i] + m_y[i+1]);
      u = (std::sqrt(h*h + 4*(1-h)*generator.uniform()) - h) / (2 * (1-h));
    }

    return m_x[i] + u * (m_x[i+1] - m_x[i]);
  }

  void Pdf::set_index_sampling() {
    std::vector<double> probabilities;
    std::vector<size_t> indexes;

    probabilities.reserve(m_x.size() - 1);
    indexes.reserve(m_x.size() - 1);

    m_total_integral = 0;

    for (size_t i = 1; i < m_x.size(); ++i) {
      probabilities.push_back((m_x[i] - m_x[i-1]) * (m_y[i] + m_y[i-1]) / 2.0);
      indexes.push_back(i-1);
      m_total_integral += probabilities.back();
    }

    for (auto& y : m_y) {
      y /= m_total_integral;
    }

    for (auto& x : probabilities) {
      x /= m_total_integral;
    }

    m_index_sampling = distributions::ProbabilitySampling<size_t>(probabilities, indexes);
  }

  void Pdf::validate_values() {
    if (m_x.size() != m_y.size()) {
      throw std::runtime_error("Pdf must have same number of x and y values");
    }

    for (size_t i = 1; i < m_x.size(); ++i) {
      if (m_x[i] < m_x[i-1]) {
        throw std::runtime_error("Pdf must have non-decreasing x values");
      }
    }

    for (size_t i = 0; i < m_y.size(); ++i) {
      if (m_y[i] < 0) {
        throw std::runtime_error("Pdf must have non-negative y values");
      }
    }
  }
}
