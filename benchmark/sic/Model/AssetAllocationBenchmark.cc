#include <benchmark/benchmark.h>

#include "sic/Base/Parallelism.hh"
#include "sic/Model/AssetAllocation.hh"
#include "sic/UseCase/TraditionalAAContext.hh"
#include "sic/UseCases.hh"

class AssetAllocationBenchmark : public benchmark::Fixture {};

BENCHMARK_DEFINE_F(AssetAllocationBenchmark,
				   EvaluatePortfolioRestrictions_BankWide)
(benchmark::State &state) {

	auto &useCase = sic::TraditionalAAContext::getSingleton();
	auto &context = useCase.getEvaluationContext();
	std::size_t maxPortfolioCount = state.range(0);
	std::size_t threadCount = state.range(1);
	sic::ParallelParameters paraPars(threadCount);

	state.counters.insert({{"portfolioCount", maxPortfolioCount},
						   {"threadCount", threadCount},
						   {"serial", false}});

	for (auto _ : state) {
		sic::UseCase::evaluateRestrictionResults(context, maxPortfolioCount,
												 paraPars);
	}
}

BENCHMARK_REGISTER_F(AssetAllocationBenchmark,
					 EvaluatePortfolioRestrictions_BankWide)
	->RangeMultiplier(2)
	->Ranges({{2 << 10, 2 << 15},
			  {1, sic::ParallelParameters::getMaxThreadCount()}});

static void
AssetAllocation_evaluatePortfolios_BankWide_serial(benchmark::State &state) {

	auto &useCase = sic::TraditionalAAContext::getSingleton();
	auto &context = useCase.getEvaluationContext();
	std::size_t maxPortfolioCount = state.range(0);
	std::size_t threadCount = 1;
	auto serial = true;
	sic::ParallelParameters paraPars(threadCount, serial);

	state.counters.insert(
		{{"portfolioCount", maxPortfolioCount}, {"serial", true}});

	for (auto _ : state) {
		sic::UseCase::evaluateRestrictionResults(context, maxPortfolioCount,
												 paraPars);
	}
}

BENCHMARK(AssetAllocation_evaluatePortfolios_BankWide_serial)
	->RangeMultiplier(2)
	->Ranges({{2 << 10, 2 << 15}});
