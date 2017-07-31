#include "roulette/sources/source_factory.h"
#include "roulette/sources/beamlet_source.h"
#include "roulette/sources/beam_source.h"
#include "roulette/sources/composite_source.h"

namespace roulette {
  namespace sources {
    class Source;

    std::shared_ptr<Source> SourceFactory::source(const rapidjson::Value& data) {
      assert(data.HasMember("type"));

      if (data["type"].GetString() == std::string("BeamletSource")) {
        return std::make_shared<BeamletSource>(data);
      }
      else if (data["type"].GetString() == std::string("BeamSource")) {
        return std::make_shared<BeamSource>(data);
      }
      else if (data["type"].GetString() == std::string("CompositeSource")) {
        return std::make_shared<CompositeSource>(data);
      }
      else {
        // Unhandled source type
        assert(data["type"].GetString());
        return std::shared_ptr<Source>();
      }
    }
  };
};
