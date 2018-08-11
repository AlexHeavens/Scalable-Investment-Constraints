#include <benchmark/benchmark.h>

#include <boost/functional/hash.hpp>

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
	->Ranges({{2 << 10, 2 << 19},
			  {1, sic::ParallelParameters::getMaxThreadCount()}});

BENCHMARK_DEFINE_F(AssetAllocationBenchmark,
				   OutputPortfolioRestrictions_serial_BankWide)
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
		sic::UseCase::outputRestrictionResults(context, maxPortfolioCount,
											   paraPars);
	}
}

BENCHMARK_REGISTER_F(AssetAllocationBenchmark,
					 OutputPortfolioRestrictions_serial_BankWide)
	->RangeMultiplier(2)
	->Ranges({{2 << 10, 2 << 19}});

BENCHMARK_DEFINE_F(AssetAllocationBenchmark,
				   OutputPortfolioRestrictions_BankWide)
(benchmark::State &state) {

	auto &useCase = sic::TraditionalAAContext::getSingleton();
	auto &context = useCase.getEvaluationContext();
	std::size_t maxPortfolioCount = state.range(0);
	std::size_t threadCount = state.range(1);
	sic::ParallelParameters paraPars(threadCount);

	state.counters.insert({{"portfolioCount", maxPortfolioCount},
						   {"threadCount", threadCount},
						   {"serial", false}});

	std::unique_ptr<std::vector<std::vector<std::string>>> result;
	for (auto _ : state) {
		result = sic::UseCase::outputRestrictionResults(
			context, maxPortfolioCount, paraPars);
	}

	// Sanity check the output.  We sort to allow for non-sequential ordering
	// due to parallelisation.
	std::vector<std::string> orderedResults;
	for (auto &resultVector : *result) {
		for (auto &resultString : resultVector) {
			orderedResults.emplace_back(resultString);
		}
	}
	std::sort(orderedResults.begin(), orderedResults.end());
	const auto hash =
		boost::hash_range(orderedResults.begin(), orderedResults.end());

	std::cout << "Result Count: " << orderedResults.size() << " hash: " << hash
			  << "\n";
}

BENCHMARK_REGISTER_F(AssetAllocationBenchmark,
					 OutputPortfolioRestrictions_BankWide)
	->RangeMultiplier(2)
	->Ranges({{2 << 10, 2 << 19},
			  {1, sic::ParallelParameters::getMaxThreadCount()}});
