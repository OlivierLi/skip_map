#include "benchmark/benchmark.h"
#include <iostream>
#include "skip_map.h"

static void BM_SkipMapCreation(benchmark::State& state) {
    while (state.KeepRunning()){
        skip_map<int, std::string> sm;
    }
}

static void BM_MapCreation(benchmark::State& state) {
    while (state.KeepRunning()){
        std::map<int, std::string> m;
    }
}

BENCHMARK(BM_SkipMapCreation);
BENCHMARK(BM_MapCreation);

BENCHMARK_MAIN();
