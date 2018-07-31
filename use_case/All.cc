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

	sic::UseCase::time("EvaluatePortfolio", [&]() {
		sic::UseCase::evaluatePortfolios(context, maxPortfolioCount);
	});

	sic::UseCase::time("EvaluateRestrictions", [&]() {
		sic::UseCase::evaluateRestrictionResults(context, maxPortfolioCount);
	});

	return 0;
}
