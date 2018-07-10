#include <chrono>
#include <iostream>
#include <utility>

#include "sic/UseCase/TraditionalAAContext.hh"
#include "sic/UseCases.hh"

int main() {

	sic::TraditionalAAContext useCase;
	auto &context = useCase.getEvaluationContext();

	auto startTime = std::chrono::high_resolution_clock::now();

	sic::UseCase::filterAssets(context);

	auto finishTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> durationMilliseconds =
		finishTime - startTime;
	std::cout << "TraditionalAAUseCase, FilterAsset, Wall Time (ms), "
			  << durationMilliseconds.count() << "\n";

	startTime = std::chrono::high_resolution_clock::now();

	sic::UseCase::evaluatePortfolios(context);

	finishTime = std::chrono::high_resolution_clock::now();
	durationMilliseconds = finishTime - startTime;
	std::cout << "TraditionalAAContext, EvaluatePortfolio, Wall Time (ms), "
			  << durationMilliseconds.count() << "\n";

	startTime = std::chrono::high_resolution_clock::now();

	sic::UseCase::evaluateRestrictionResults(context);

	finishTime = std::chrono::high_resolution_clock::now();
	durationMilliseconds = finishTime - startTime;
	std::cout
		<< "TraditionalAAContext, EvaluateRestrictionResults, Wall Time (ms), "
		<< durationMilliseconds.count() << "\n";

	return 0;
}
