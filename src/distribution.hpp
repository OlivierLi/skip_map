#pragma once

#include <algorithm>
#include <random>

/// This class implements an exponential distribution.  The point is to have
/// decreasing odds to get larger levels to actually get the traversal behavior
/// we want.
class Distribution {
 public:
  Distribution() : gen{rd()}, d{0.8} {}

  size_t get_value() {
    return static_cast<size_t>(std::clamp(d(gen), 0.0, 3.0));
  }

 private:
  std::random_device rd;
  std::mt19937 gen;
  std::exponential_distribution<> d;
};
