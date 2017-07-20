#include <gtest/gtest.h>

#include "real_function_test.h"
#include "distributions/uniform_test.h"

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
