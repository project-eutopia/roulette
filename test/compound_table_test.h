#pragma once

#include "test_helper.h"

#include "roulette/compound_table.h"

using namespace roulette;

TEST(CompoundTableTest, reads_in_from_json_file) {
  CompoundTable compound_table("../data/compounds.json");

  const Element& hydrogen = compound_table.periodic_table().element(1);
  const Element& oxygen = compound_table.periodic_table().element(8);
  const Compound& water = compound_table.compound("Water, Liquid");

  double hydrogen_ratio = water.composition().at(1);
  double oxygen_ratio = water.composition().at(8);

  EXPECT_NEAR(
    hydrogen_ratio*hydrogen.photon_scattering_cross_section(123456) + oxygen_ratio*oxygen.photon_scattering_cross_section(123456),
    water.photon_scattering_cross_section(123456),
    0.0000001
  );
}
