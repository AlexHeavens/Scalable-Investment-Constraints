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

BENCHMARK_DEFINE_F(AssetAllocationBenchmark,
				   EvaluatePortfolioRestrictions_serial_BankWide)
(benchmark::State &state) {

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

BENCHMARK_REGISTER_F(AssetAllocationBenchmark,
					 EvaluatePortfolioRestrictions_serial_BankWide)
	->RangeMultiplier(2)
	->Ranges({{2 << 10, 2 << 15}});

BENCHMARK_DEFINE_F(AssetAllocationBenchmark,
				   OutputPortfolioRestrictions_BankWide)
(benchmark::State &state) {

	auto &useCase = sic::TraditionalAAContext::getSingleton();
	auto &context = useCase.getEvaluationContext();
	std::size_t maxPortfolioCount = state.range(0);
	std::size_t threadCount = state.range(1);
	sic::ParallelParameters paraPars(threadCount);

	std::vector<std::vector<std::string>> globalResultStrings;
	globalResultStrings.resize(threadCount);

	std::unique_ptr<sic::AbstractAssetAllocation::ResultVector>
		results[maxPortfolioCount];

	state.counters.insert({{"portfolioCount", maxPortfolioCount},
						   {"threadCount", threadCount},
						   {"serial", false}});

	for (auto _ : state) {
		sic::UseCase::outputRestrictionResults(context, maxPortfolioCount,
											   paraPars, &globalResultStrings,
											   results);
	}

	unused(globalResultStrings);
}

BENCHMARK_REGISTER_F(AssetAllocationBenchmark,
					 OutputPortfolioRestrictions_BankWide)
	->RangeMultiplier(2)
	->Ranges({{2 << 10, 2 << 15},
			  {1, sic::ParallelParameters::getMaxThreadCount()}});
