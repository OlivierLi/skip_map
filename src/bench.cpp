#include <iostream>
#include <list>
#include "benchmark/benchmark.h"
#include "skip_map.h"
#include "test_facilities.hpp"

// TODO : Print structure to visualize problems, each level seems to be doing a
// linear search this is like if all the level were full linked lists
// TODO : Count allocations of objects through ctor calls
// TODO : Different test using map of 10000 element already created

static void BM_SkipMapCreation(benchmark::State& state) {
  while (state.KeepRunning()) {
    skip_map<Key, Value> sm;
    fill(sm);
  }
}

static void BM_MapCreation(benchmark::State& state) {
  while (state.KeepRunning()) {
    std::map<Key, Value> m;
    fill(m);
  }
}

static void BM_FixedVectorCreation(benchmark::State& state) {
  while (state.KeepRunning()) {
    fixed_vector<int, MAX_LEVEL> f;
    for (int i = 0; i < MAX_LEVEL; ++i) {
      f.push_back(i);
      benchmark::DoNotOptimize(f.at(i));
    }
  }
}

static void BM_VectorCreation(benchmark::State& state) {
  while (state.KeepRunning()) {
    std::vector<int> v;
    for (int i = 0; i < MAX_LEVEL; ++i) {
      v.push_back(i);
      benchmark::DoNotOptimize(v.at(i));
    }
  }
}

class MyFixture : public benchmark::Fixture {
 public:
  void SetUp(const ::benchmark::State& /*state*/) {
    // Fill a skip_map making sure that it looks like a linked-list as much as
    // possible.
    sm.set_gen_for_testing([]() { return 0; });
    fill(sm);

    // Fill an equivalent linked-list.
    l = std::list<KeyValue>(default_size, {1, long_string});
  }

  skip_map<Key, Value> sm;
  std::list<KeyValue> l;
};

BENCHMARK_F(MyFixture, BM_ListIterate)(benchmark::State& state) {
  while (state.KeepRunning()) {
    for (auto it = l.cbegin(); it != l.cend(); ++it)
      ;
  }
}

BENCHMARK_F(MyFixture, BM_SkipMapIterate)(benchmark::State& state) {
  while (state.KeepRunning()) {
    for (auto it = sm.cbegin(); it != sm.cend(); ++it)
      ;
  }
}

BENCHMARK(BM_SkipMapCreation);
BENCHMARK(BM_MapCreation);

BENCHMARK(BM_FixedVectorCreation);
BENCHMARK(BM_VectorCreation);

BENCHMARK_MAIN();
