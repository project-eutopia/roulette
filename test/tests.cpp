#include <gtest/gtest.h>

#include "math_test.h"
#include "real_function_test.h"
#include "four_momentum_test.h"
#include "photon_test.h"
#include "electron_test.h"
#include "regular_voxel_grid_test.h"
#include "irregular_voxel_grid_test.h"
#include "phantom_test.h"
#include "dose_calculation_test.h"
#include "beamlet_test.h"
#include "non_uniform_linear_interpolation_test.h"
#include "compound_table_test.h"
#include "super_linear_interpolation_test.h"
#include "interaction_function_test.h"
#include "rotation_matrix_test.h"
#include "affine_transformation_test.h"
#include "distributions/uniform_test.h"
#include "distributions/exponential_test.h"
#include "distributions/rejection_sampling_test.h"
#include "distributions/compton_scattering_test.h"
#include "geometries/point_test.h"
#include "geometries/rectangle_test.h"
#include "geometries/ellipse_test.h"
#include "geometries/spherical_shell_test.h"
#include "geometries/hemispherical_shell_test.h"
#include "geometries/geometry_factory_test.h"
#include "sources/bifocal_source_test.h"

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
