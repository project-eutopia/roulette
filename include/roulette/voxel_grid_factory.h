#pragma once

#include <memory>

#include "roulette/i_voxel_grid.h"

#include "rapidjson/document.h"

namespace roulette {
  class VoxelGridFactory {
    public:
      static std::shared_ptr<IVoxelGrid> from_json(const rapidjson::Value& data);
  };
}
