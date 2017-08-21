#include "benchmark/benchmark.h"
#include <iostream>
#include "skip_map.h"

// TODO : Print structure to visualize problems, each level seems to be doing a linear search
// this is like if all the level were full linked lists
// TODO : Count allocations of objects through ctor calls
// TODO : Different test using map of 10000 element already created

template<typename C>
void fill(C& container){
  for(size_t i=0;i<1000;++i){
    container.insert({i, "This is a very long long long string"});
  }
}

static void BM_SkipMapCreation(benchmark::State& state) {
    while (state.KeepRunning()){
        skip_map<int, const char*> sm;
        fill(sm);
    }
}

static void BM_MapCreation(benchmark::State& state) {
    while (state.KeepRunning()){
        std::map<int, const char*> m;
        fill(m);
    }
}

BENCHMARK(BM_SkipMapCreation);
BENCHMARK(BM_MapCreation);

BENCHMARK_MAIN();
