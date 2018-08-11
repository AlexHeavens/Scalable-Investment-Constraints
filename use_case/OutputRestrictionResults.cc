#include "sic/Base/Variable.hh"
#include "sic/UseCase/TraditionalAAContext.hh"
#include "sic/UseCases.hh"

int main() {

	sic::TraditionalAAContext useCase;
	auto &context = useCase.getEvaluationContext();
	std::size_t maxPortfolioCount = sic::UseCase::rebalancerPortfolioCount;

	sic::ParallelParameters paraPars;
	const auto threadCount = paraPars.threadCount;

	std::vector<std::vector<std::string>> globalResultStrings;
	globalResultStrings.resize(threadCount);

	std::unique_ptr<sic::AbstractAssetAllocation::ResultVector>
		results[maxPortfolioCount];

	sic::UseCase::time("OutputRestrictionResults", [&]() {
		sic::UseCase::outputRestrictionResults(context, maxPortfolioCount,
											   paraPars, &globalResultStrings,
											   results);
	});

	return 0;
}
