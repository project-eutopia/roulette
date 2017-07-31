#include "roulette/distributions/spectrum.h"
#include "roulette/json.h"

#include <iostream>
#include "rapidjson/document.h"

namespace roulette {
  namespace distributions {
    Spectrum::Spectrum() {}

    Spectrum::Spectrum(std::string filename_or_json_string) :
      m_inv_cdf()
    {
      const rapidjson::Document data = Json::json_document_from_file_or_string(filename_or_json_string);

      const rapidjson::Value& array = data["data"];

      for (auto it = array.Begin(); it != array.End(); ++it) {
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
    };

    double Spectrum::operator()(RandomGenerator& generator) {
      return m_inv_cdf(generator.uniform());
    };
  };
};
