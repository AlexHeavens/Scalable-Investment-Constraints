#include "sic/UseCase/TraditionalAAContext.hh"
#include "sic/UseCases.hh"

int main() {

	sic::TraditionalAAContext useCase;
	auto &context = useCase.getEvaluationContext();
	std::size_t maxPortfolioCount = sic::UseCase::rebalancerPortfolioCount;

	sic::UseCase::time("EvaluateRestrictions", [&]() {
		sic::UseCase::evaluateRestrictionResults(context, maxPortfolioCount);
	});

	return 0;
}
