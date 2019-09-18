#pragma once

constexpr const char* long_string{"This is a very long long long string"};
constexpr int default_size{1000};
constexpr int MAX_LEVEL = 4;

using Key = int;
using Value = const char*;
using KeyValue = std::pair<Key, Value>;

template <typename T>
struct compare_with_stats {
  constexpr bool operator()(const T& lhs, const T& rhs) const {
    ++compare_count;
    return std::less<T>()(lhs, rhs);
  }

  // We tolerate a mutable variable here as it is only for testing.
  mutable size_t compare_count{0};
};

// Fil a container with increasingly large keys and the same value.
template <typename C>
void fill(C& container) {
  for (size_t i = 0; i < default_size; ++i) {
    container.insert({i, long_string});
  }
}
