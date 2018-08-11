#include <benchmark/benchmark.h>

#include <boost/functional/hash.hpp>

#include "sic/Model/AssetAllocation.hh"
#include "sic/UseCase/TraditionalAAContext.hh"
#include "sic/UseCases.hh"

class AssetAllocationBenchmark : public benchmark::Fixture {};

BENCHMARK_DEFINE_F(AssetAllocationBenchmark,
				   OutputPortfolioRestrictions_BankWide)
(benchmark::State &state) {

	auto &useCase = sic::TraditionalAAContext::getSingleton();
	auto &context = useCase.getEvaluationContext();
	std::size_t maxPortfolioCount = state.range(0);

	state.counters.insert({{"portfolioCount", maxPortfolioCount}});

	std::unique_ptr<std::vector<std::string>> result;
	for (auto _ : state) {
		result =
			sic::UseCase::outputRestrictionResults(context, maxPortfolioCount);
	}

	// Sanity check the output.  We sort to allow for non-sequential ordering
	// due to parallelisation.
	std::sort(result->begin(), result->end());
	const auto hash = boost::hash_range(result->begin(), result->end());

	std::cout << "Result Count: " << result->size() << " hash: " << hash
			  << "\n";
}

BENCHMARK_REGISTER_F(AssetAllocationBenchmark,
					 OutputPortfolioRestrictions_BankWide)
	->RangeMultiplier(2)
	->Ranges({{2 << 10, 2 << 19}});
