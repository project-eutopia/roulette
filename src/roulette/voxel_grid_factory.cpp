#include "roulette/voxel_grid_factory.h"
#include "roulette/voxel_grid.h"

namespace roulette {
  std::shared_ptr<IVoxelGrid> VoxelGridFactory::from_json(const rapidjson::Value& data) {
    if (!data.HasMember("type") || !data["type"].IsString()) {
      throw InvalidVoxelGrid("Voxel grid must have \"type\"");
    }

    if (data["type"].GetString() == std::string("RegularVoxelGrid")) {
      return std::make_shared<VoxelGrid>(data);
    }
    else {
      throw InvalidVoxelGrid("Unknown voxel grid type");
    }
  }
}
