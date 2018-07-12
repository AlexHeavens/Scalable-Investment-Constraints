#include <benchmark/benchmark.h>

#include "sic/Model/AssetAllocation.hh"
#include "sic/UseCase/TraditionalAAContext.hh"
#include "sic/UseCases.hh"

static void
AssetAllocation_evaluatePortfolios_BankWide(benchmark::State &state) {

	auto &useCase = sic::TraditionalAAContext::getSingleton();
	auto &context = useCase.getEvaluationContext();
	std::size_t maxPortfolioCount = state.range(0);

	state.counters.insert({{"portfolioCount", maxPortfolioCount}});

	for (auto _ : state) {
		sic::UseCase::evaluateRestrictionResults(context, maxPortfolioCount);
	}
}

BENCHMARK(AssetAllocation_evaluatePortfolios_BankWide)
	->RangeMultiplier(2)
	->Ranges({{2 << 10, 2 << 19}});
