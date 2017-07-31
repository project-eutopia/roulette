#include "roulette/sources/composite_source.h"
#include "roulette/sources/source_factory.h"
#include "roulette/particle.h"

#include <stdexcept>

namespace roulette {
  namespace sources {
    CompositeSource::CompositeSource(const rapidjson::Value& data) {
      assert(data["type"].GetString() == std::string("CompositeSource"));

      if (!data.HasMember("sub_sources")) throw std::runtime_error("CompositeSource requires \"sub_sources\"");
      const rapidjson::Value& sub_sources = data["sub_sources"];
      if (!sub_sources.IsArray()) throw std::runtime_error("CompositeSource \"sub_sources\" must be array");

      double total_fractions = 0;
      double fraction;

      for (auto it = sub_sources.Begin(); it != sub_sources.End(); ++it) {
        if (!it->HasMember("fraction") || !(*it)["fraction"].IsDouble()) throw std::runtime_error("CompositeSource sub_source must have double member \"fraction\"");

        fraction = (*it)["fraction"].GetDouble();
        total_fractions += fraction;
        m_fractions.push_back(fraction);

        if (!it->HasMember("source")) throw std::runtime_error("CompositeSource sub_source must have member \"source\"");
        m_sub_sources.push_back(SourceFactory::source((*it)["source"]));
      }

      // Re-normalize to total fraction of 1.0
      for (auto& fraction : m_fractions) fraction /= total_fractions;

      m_inverse_cdf_to_sub_source_index.add_point(0, 0);

      for (int i = 0; i < m_fractions.size(); ++i) {
        m_inverse_cdf_to_sub_source_index.add_point(
          m_fractions[i] + m_inverse_cdf_to_sub_source_index.xs().back(),
          i+1
        );
      }
    }

    std::shared_ptr<Particle> CompositeSource::particle(RandomGenerator& generator) {
      int i = m_inverse_cdf_to_sub_source_index(generator.uniform());
      return m_sub_sources[i]->particle(generator);
    }
  };
};
