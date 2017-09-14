#include "roulette/distributions/spectrum.h"
#include "roulette/json.h"

#include <iostream>

// TODO: spectrum types (histogram, dirac delta, composition)

namespace roulette {
  namespace distributions {
    Spectrum::Spectrum() {}

    Spectrum::Spectrum(const rapidjson::Value& data)
    {
      if (data.IsString()) {
        const rapidjson::Document data2 = Json::json_document_from_file_or_string(data.GetString());
        this->load_data(data2);
      }
      else {
        this->load_data(data);
      }
    }

    Spectrum::Spectrum(std::string filename_or_json_string)
    {
      const rapidjson::Document data = Json::json_document_from_file_or_string(filename_or_json_string);
      this->load_data(data);
    };

    double Spectrum::operator()(RandomGenerator& generator) {
      if (m_value > 0) {
        return m_value;
      }
      else {
        return m_inv_cdf(generator.uniform());
      }
    };

    void Spectrum::load_data(const rapidjson::Value& spectrum) {
      if (!spectrum.HasMember("type") || !spectrum["type"].IsString()) throw std::runtime_error("Spectrum must have \"type\" string");

      if (spectrum["type"].GetString() == std::string("pdf")) {
        this->load_pdf(spectrum);
      }
      else if (spectrum["type"].GetString() == std::string("delta")) {
        this->load_delta(spectrum);
      }
    }

    void Spectrum::load_pdf(const rapidjson::Value& spectrum) {
      if (!spectrum.HasMember("data") || !spectrum["data"].IsArray()) throw std::runtime_error("PDF Spectrum must have \"data\" array");
      const rapidjson::Value& data = spectrum["data"];

      for (auto it = data.Begin(); it != data.End(); ++it) {
        m_pdf.add_point(
            (*it)[0].GetDouble() * 1000000, // Convert from MeV to eV
            (*it)[1].GetDouble()
        );
      }

      m_cdf.add_point(m_pdf.xs().front(), 0);

      for (int i = 1; i < m_pdf.xs().size(); ++i) {
        m_cdf.add_point(
          m_pdf.xs()[i],
          m_cdf.ys()[i-1] + (m_pdf.ys()[i-1] + m_pdf.ys()[i])/2.0 * (m_pdf.xs()[i] - m_pdf.xs()[i-1])
        );
      }

      double normalization = m_cdf.ys().back();

      for (int i = 0; i < m_cdf.xs().size(); ++i) {
        m_inv_cdf.add_point(
          m_cdf.ys()[i] / normalization,
          m_cdf.xs()[i]
        );
      }
      m_value = 0;
    }

    void Spectrum::load_delta(const rapidjson::Value& spectrum) {
      if (!spectrum.HasMember("value") || !spectrum["value"].IsNumber()) throw std::runtime_error("Delta Spectrum must have \"value\" number");
      m_value = spectrum["value"].GetDouble() * 1000000;
    }
  };
};
