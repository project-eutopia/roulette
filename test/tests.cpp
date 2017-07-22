#include <gtest/gtest.h>

#include "real_function_test.h"
#include "four_momentum_test.h"
#include "distributions/uniform_test.h"
#include "distributions/exponential_test.h"
#include "distributions/rejection_sampling_test.h"
#include "distributions/compton_scattering_test.h"

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
