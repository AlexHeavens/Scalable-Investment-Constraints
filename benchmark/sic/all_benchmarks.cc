#include <benchmark/benchmark.h>

static void BM_Dummy(benchmark::State &state) {
	for (auto _ : state)
		std::string empty_string;
}

BENCHMARK(BM_Dummy);

BENCHMARK_MAIN();
