#pragma once

#include <memory>

#include "test_helper.h"

#include "roulette/simulation.h"

using namespace roulette;

TEST(SimulationTest, simulation_produces_dose) {
  Simulation simulation(" \
{ \
  \"description\": \"Test run\", \
  \"compound_table\": \"../data/compounds.json\", \
  \"phantom\": { \
    \"voxel_grid\": { \
      \"nx\": 1, \
      \"ny\": 1, \
      \"nz\": 1, \
      \"bottom_left\": [0, -25, -25], \
      \"top_right\": [50, 25, 25] \
    }, \
    \"density\": 1.06, \
    \"compound\": \"Water, Liquid\" \
  }, \
  \"seed\": 1234, \
  \"source_simulations\": [ \
    { \
      \"number_of_particles\": 1, \
      \"weight\": 1.0, \
      \"source\": { \
        \"type\": \"BeamletSource\", \
        \"source\": [-100, 0, 0], \
        \"bottom_left\": [-50, -5, -5], \
        \"bottom_right\": [-50, 5, -5], \
        \"top_right\": [-50, 5, 5], \
        \"spectrum\": {\"data\": [[5.0, 0], [5.5, 0.5], [6.0, 1]]} \
      } \
    } \
  ] \
} \
  ");

  simulation.run();
  EXPECT_GT(simulation.source_simulations()[0].dose()(0,0,0), 0);
}
