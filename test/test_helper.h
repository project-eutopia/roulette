#pragma once

#include <gtest/gtest.h>
#include "support/test_multiple_times.h"

#include "roulette/compound_table.h"

namespace roulette {
  // Global variables
  const CompoundTable compound_table("../data/compounds.json");
};
