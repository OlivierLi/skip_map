/// This executable is intended as a simple way to execute precisely the code we
/// want to inspect using Linux Perf.

// See cycles difference  : sudo perf record -e cycles:u -g -- ./events
// See misses difference  : sudo perf record -e LLC-misses -c 10 -g -- ./events

// Analyze using : sudo perf report --symbol-filter=iterate

#include <iostream>
#include <list>
#include "benchmark/benchmark.h"
#include "skip_map.h"
#include "test_facilities.hpp"

size_t large_size = 10000;

void __attribute__((noinline)) iterateSkipMap(skip_map<Key, Value>& sm) {
  for (auto it = sm.begin(); it != sm.end();) {
    benchmark::DoNotOptimize(++it);
  }
}

void __attribute__((noinline))
iterateList(std::list<std::pair<Key, Value>>& list) {
  for (auto it = list.begin(); it != list.end();)
    benchmark::DoNotOptimize(++it);
}

int main(int argc, char**) {
  // Create a list. Initiliaze key to argc to prevent some optimizations maybe.
  std::list<KeyValue> list(large_size, {argc, long_string});

  // Create an equivalent skip_map.
  skip_map<Key, Value> sm;
  sm.set_gen_for_testing([]() { return 0; });
  fill(sm, large_size);

  for (int i = 0; i < 1000; ++i) {
    iterateSkipMap(sm);
    iterateList(list);
  }

  return 0;
}
