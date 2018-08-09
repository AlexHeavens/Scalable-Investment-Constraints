#include "sic/UseCase/TraditionalAAContext.hh"
#include "sic/UseCases.hh"

int main() {

	sic::TraditionalAAContext useCase;
	auto &context = useCase.getEvaluationContext();
	std::size_t maxPortfolioCount = sic::UseCase::rebalancerPortfolioCount;

	sic::UseCase::timeUseCase(
		[&]() { sic::UseCase::evaluatePortfolios(context, maxPortfolioCount); },
		"EvaluatePortfolio");

	return 0;
}
