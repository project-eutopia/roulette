#include <gtest/gtest.h>

#include "real_function_test.h"
#include "four_momentum_test.h"
#include "photon_test.h"
#include "voxel_grid_test.h"
#include "density_grid_test.h"
#include "beamlet_test.h"
#include "non_uniform_linear_interpolation_test.h"
#include "distributions/uniform_test.h"
#include "distributions/exponential_test.h"
#include "distributions/rejection_sampling_test.h"
#include "distributions/compton_scattering_test.h"

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
