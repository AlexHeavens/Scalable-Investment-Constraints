#include "sic/UseCase/TraditionalAAContext.hh"
#include "sic/UseCases.hh"

int main() {

	sic::TraditionalAAContext useCase;
	auto &context = useCase.getEvaluationContext();
	std::size_t maxPortfolioCount = context.getPortfolioCache().size();
	std::size_t maxAssetCount = context.getAssetCache().size();
	std::size_t maxFilterTreeCount = context.getFilterTreeCache().size();

	sic::UseCase::timeUseCase(
		[&]() {
			sic::UseCase::filterAssets(context, maxFilterTreeCount,
									   maxAssetCount);
		},
		"FilterAssets");

	sic::UseCase::timeUseCase(
		[&]() { sic::UseCase::evaluatePortfolios(context, maxPortfolioCount); },
		"EvaluatePortfolio");

	sic::UseCase::timeUseCase(
		[&]() {
			sic::UseCase::evaluateRestrictionResults(context,
													 maxPortfolioCount);
		},
		"EvaluateRestrictions");

	return 0;
}
