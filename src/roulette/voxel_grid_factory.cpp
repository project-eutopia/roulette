#include "roulette/voxel_grid_factory.h"
#include "roulette/voxel_grid.h"

namespace roulette {
  std::shared_ptr<IVoxelGrid> VoxelGridFactory::from_json(const rapidjson::Value& data) {
    return std::make_shared<VoxelGrid>(data);
  }
}
