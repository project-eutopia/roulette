#pragma once

#include <map>
#include "roulette/element.h"

#include "rapidjson/document.h"

namespace roulette {
  class PeriodicTable {
    private:
      std::map<int,Element> m_elements;

    public:
      PeriodicTable(const rapidjson::Value& data);

      const std::map<int,Element>& elements() const;
      const Element& element(int atomic_number) const;
  };
};
