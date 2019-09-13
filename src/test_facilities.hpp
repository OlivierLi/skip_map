#pragma once

template <typename T>
struct compare_with_stats {
  constexpr bool operator()(const T& lhs, const T& rhs) const {
    ++compare_count;
    return std::less<T>()(lhs, rhs);
  }

  // We tolerate a mutable variable here as it is only for testing.
  mutable size_t compare_count{0};
};
