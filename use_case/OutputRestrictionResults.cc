#include "sic/Base/Variable.hh"
#include "sic/UseCase/TraditionalAAContext.hh"
#include "sic/UseCases.hh"

int main() {

	sic::TraditionalAAContext useCase;
	auto &context = useCase.getEvaluationContext();
	std::size_t maxPortfolioCount = sic::UseCase::rebalancerPortfolioCount;

	sic::UseCase::time("OutputRestrictionResults", [&]() {
		auto results =
			sic::UseCase::outputRestrictionResults(context, maxPortfolioCount);
		unused(results);
	});

	return 0;
}
