#pragma once

#include <memory>

#include "test_helper.h"

#include "roulette/dose_calculation.h"

using namespace roulette;

TEST(DoseCalculationTest, dose_calculation_produces_dose) {
  DoseCalculation dose_calculation(" \
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
  \"density_compound_map\": \"Water, Liquid\", \
  \"dose_storage\": \"matrix\", \
  \"source_doses\": [ \
    { \
      \"number_of_particles\": 50, \
      \"weight\": 1.0, \
      \"source\": { \
        \"type\": \"BeamletSource\", \
        \"source\": [-100, 0, 0], \
        \"bottom_left\": [-50, -5, -5], \
        \"bottom_right\": [-50, 5, -5], \
        \"top_right\": [-50, 5, 5], \
        \"spectrum\": {\"type\": \"pdf\", \"data\": [[5.0, 0], [5.5, 0.5], [6.0, 1]]} \
      } \
    } \
  ] \
} \
  ");

  auto dose = dose_calculation.source_doses()[0]->dose();
  EXPECT_GT((*dose)(0,0,0), 0);
}

TEST(DoseCalculationTest, dose_calculation_produces_dose_with_pointwise_storage) {
  DoseCalculation dose_calculation(" \
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
  \"density_compound_map\": \"Water, Liquid\", \
  \"dose_storage\": {\"type\": \"pointwise\", \"points\": [[25, 0, 0]]}, \
  \"source_doses\": [ \
    { \
      \"number_of_particles\": 50, \
      \"weight\": 1.0, \
      \"source\": { \
        \"type\": \"BeamletSource\", \
        \"source\": [-100, 0, 0], \
        \"bottom_left\": [-50, -5, -5], \
        \"bottom_right\": [-50, 5, -5], \
        \"top_right\": [-50, 5, 5], \
        \"spectrum\": {\"type\": \"pdf\", \"data\": [[5.0, 0], [5.5, 0.5], [6.0, 1]]} \
      } \
    } \
  ] \
} \
  ");

  auto dose = dose_calculation.source_doses()[0]->dose();
  EXPECT_GT((*dose)(0,0,0), 0);
}
