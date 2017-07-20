#pragma once

#include <gtest/gtest.h>
#include <functional>

void test_multiple_times(std::function<bool(void)> trial, int attempts) {
  for (int i = 0; i < attempts; ++i) {
    bool attempt = trial();
    // Success!
    if (attempt) return;
  }

  ASSERT_TRUE(false);
}
