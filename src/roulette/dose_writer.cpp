#include "roulette/dose_writer.h"

#include "roulette/json.h"

namespace roulette {
  DoseWriter::DoseWriter() : m_type(WriterType::MATRIX)
  {
  }

  DoseWriter::DoseWriter(const rapidjson::Value& data) {
    if (!data.HasMember("type") || !data["type"].IsString()) throw std::runtime_error("DoseWriter[\"type\"] should be string");

    if (data["type"].GetString() == std::string("matrix")) {
      m_type = WriterType::MATRIX;
    }
    else if (data["type"].GetString() == std::string("map")) {
      m_type = WriterType::MAP;
      if (!data.HasMember("cutoff_fraction") || !data["cutoff_fraction"].IsNumber()) throw std::runtime_error("DoseWriter of type \"map\" should have \"cutoff_fraction\" number");
      m_map_cutoff_fraction = data["cutoff_fraction"].GetDouble();
    }
    else {
      throw std::runtime_error("Unknown \"type\" for DoseWriter");
    }
  }

  DoseWriter::DoseWriter(std::string filename_or_json_string) :
    DoseWriter(Json::json_document_from_file_or_string(filename_or_json_string))
  {
  }

  std::ofstream& DoseWriter::write_dose_to_file(const ThreeTensor& dose, std::ofstream& ofs) const {
    if (m_type == WriterType::MATRIX) {
      dose.write(ofs);
    }
    else if (m_type == WriterType::MAP) {
      double max_dose = 0;
      for (int zi = 0; zi < dose.nz(); ++zi) {
        for (int yi = 0; yi < dose.ny(); ++yi) {
          for (int xi = 0; xi < dose.nx(); ++xi) {
            max_dose = std::max(max_dose, dose(xi, yi, zi));
          }
        }
      }

      double cutoff = max_dose * m_map_cutoff_fraction;

      int32_t n;
      float d;

      for (int zi = 0; zi < dose.nz(); ++zi) {
        for (int yi = 0; yi < dose.ny(); ++yi) {
          for (int xi = 0; xi < dose.nx(); ++xi) {
            if (dose(xi, yi, zi) > cutoff) {
              n = xi;
              ofs.write(reinterpret_cast<const char*>(&n), sizeof(n));
              n = yi;
              ofs.write(reinterpret_cast<const char*>(&n), sizeof(n));
              n = zi;
              ofs.write(reinterpret_cast<const char*>(&n), sizeof(n));
              d = dose(xi, yi, zi);
              ofs.write(reinterpret_cast<const char*>(&d), sizeof(d));
            }
          }
        }
      }
    }

    return ofs;
  }
}
