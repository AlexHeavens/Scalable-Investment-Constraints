#include <benchmark/benchmark.h>

#include "sic/Base/Parallelism.hh"
#include "sic/Model/AssetAllocation.hh"
#include "sic/UseCase/TraditionalAAContext.hh"
#include "sic/UseCases.hh"

static void
AssetAllocation_evaluatePortfolios_BankWide(benchmark::State &state) {

	auto &useCase = sic::TraditionalAAContext::getSingleton();
	auto &context = useCase.getEvaluationContext();
	std::size_t maxPortfolioCount = state.range(0);
	std::size_t threadCount = state.range(1);
	sic::ParallelParameters paraPars(threadCount);

	state.counters.insert(
		{{"portfolioCount", maxPortfolioCount}, {"threadCount", threadCount}});

	for (auto _ : state) {
		sic::UseCase::evaluateRestrictionResults(context, maxPortfolioCount,
												 paraPars);
	}
}

BENCHMARK(AssetAllocation_evaluatePortfolios_BankWide)
	->RangeMultiplier(2)
	->Ranges({{2 << 10, 2 << 19},
			  {1, sic::ParallelParameters::getMaxThreadCount()}});
