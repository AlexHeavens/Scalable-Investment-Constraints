#include "sic/Base/Variable.hh"
#include "sic/UseCase/TraditionalAAContext.hh"
#include "sic/UseCases.hh"

int main() {

	sic::TraditionalAAContext useCase;
	auto &context = useCase.getEvaluationContext();
	std::size_t maxPortfolioCount = sic::UseCase::rebalancerPortfolioCount;
	std::size_t maxAssetCount = context.getAssetCache().size();
	std::size_t maxFilterTreeCount = context.getFilterTreeCache().size();

	sic::UseCase::time("FilterAssets", [&]() {
		sic::UseCase::filterAssets(context, maxFilterTreeCount, maxAssetCount);
	});

	sic::UseCase::time("EvaluatePortfolios", [&]() {
		sic::UseCase::evaluatePortfolios(context, maxPortfolioCount);
	});

	sic::UseCase::time("OutputRestrictionResults", [&]() {
		auto results =
			sic::UseCase::outputRestrictionResults(context, maxPortfolioCount);
		unused(results);
	});

	return 0;
}
