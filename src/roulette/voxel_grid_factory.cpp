#include "roulette/voxel_grid_factory.h"
#include "roulette/regular_voxel_grid.h"
#include "roulette/irregular_voxel_grid.h"

namespace roulette {
  std::shared_ptr<IVoxelGrid> VoxelGridFactory::from_json(const rapidjson::Value& data) {
    if (!data.HasMember("type") || !data["type"].IsString()) {
      throw InvalidVoxelGrid("Voxel grid must have \"type\"");
    }

    if (data["type"].GetString() == std::string("RegularVoxelGrid")) {
      return std::make_shared<RegularVoxelGrid>(data);
    }
    if (data["type"].GetString() == std::string("IrregularVoxelGrid")) {
      return std::make_shared<IrregularVoxelGrid>(data);
    }
    else {
      throw InvalidVoxelGrid("Unknown voxel grid type");
    }
  }
}
