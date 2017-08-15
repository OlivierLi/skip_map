#include "benchmark/benchmark.h"
#include <iostream>
#include "skip_map.h"

template<typename C>
void fill(C& container){
  for(size_t i=0;i<1000;++i){
    container.insert({i, "This is a very long long long string"});
  }
}

static void BM_SkipMapCreation(benchmark::State& state) {
    while (state.KeepRunning()){
        skip_map<int, std::string> sm;
        fill(sm);
    }
}

static void BM_MapCreation(benchmark::State& state) {
    while (state.KeepRunning()){
        std::map<int, std::string> m;
        fill(m);
    }
}

BENCHMARK(BM_SkipMapCreation);
BENCHMARK(BM_MapCreation);

BENCHMARK_MAIN();
