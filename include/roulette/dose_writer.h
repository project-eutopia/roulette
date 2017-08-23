#pragma once

#include <string>
#include <fstream>
#include <memory>
#include "roulette/three_tensor.h"
#include "rapidjson/document.h"

namespace roulette {
  class DoseWriter {
    public:
      enum WriterType {
        MATRIX,
        MAP
      };

    private:
      WriterType m_type;
      double m_map_cutoff_fraction;

    public:
      DoseWriter();
      DoseWriter(const rapidjson::Value& data);
      DoseWriter(std::string filename_or_json_string);

      std::ofstream& write_dose_to_file(std::shared_ptr<ThreeTensor> dose, std::ofstream& ofs) const;
  };
}
