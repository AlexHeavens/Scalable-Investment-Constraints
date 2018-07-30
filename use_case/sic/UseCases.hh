#ifndef SIC_USECASES_H_
#define SIC_USECASES_H_

#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

#include "sic/Base/Parallelism.hh"
#include "sic/Base/Variable.hh"
#include "sic/EvaluationContext.hh"

namespace sic {

namespace UseCase {

void timeUseCase(std::function<void()> useCase, const std::string &name) {

	auto startTime = std::chrono::high_resolution_clock::now();

	useCase();

	auto finishTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> durationMilliseconds =
		finishTime - startTime;
	std::cout << "TraditionalAAUseCase," << name << ",Wall Time (ms), "
			  << durationMilliseconds.count() << "\n";
}

void evaluateRestrictionResultsCore(
	sic::EvaluationContext &context, std::size_t maxPortfolioCount,
	const sic::ParallelParameters &paraPars = sic::ParallelParameters(),
	std::unique_ptr<sic::AbstractAssetAllocation::ResultVector> results[] =
		nullptr) {

	std::vector<std::thread> threads;

	std::size_t threadCount = paraPars.threadCount;

	std::function<void(const int)> threadEvaluatePortfolio =
		[&](const std::size_t threadID) {
			std::size_t portfolioCount = 0;
			for (const auto &portfolio : context.getPortfolioCache()) {

				if (portfolioCount >= maxPortfolioCount) {
					break;
				}

				if (paraPars.serial) {

					for (auto &aa : portfolio->getAssetAllocations()) {

						results[portfolioCount] =
							aa->generateRestrictionResults(*portfolio);
					}

				} else {

					if (portfolioCount % threadCount == threadID) {

						for (const auto &aa :
							 portfolio->getAssetAllocations()) {

							results[portfolioCount] =
								aa->generateRestrictionResults(*portfolio);
						}
					}
				}
				portfolioCount++;
			}
		};

	if (paraPars.serial) {
		threadEvaluatePortfolio(0);
	} else {
		for (std::size_t threadID = 0; threadID < paraPars.threadCount;
			 threadID++) {
			threads.emplace_back(threadEvaluatePortfolio, threadID);
		}

		for (auto &thread : threads) {
			thread.join();
		}
	}
}

void evaluatePortfolios(sic::EvaluationContext &context,
						std::size_t maxPortfolioCount) {

	std::size_t portfolioCount = 0;

	for (const auto &portfolio : context.getPortfolioCache()) {
		if (portfolioCount > maxPortfolioCount) {
			break;
		}

		for (const auto &aa : portfolio->getAssetAllocations()) {
			const auto &filterTree = aa->getFilterTree();
			auto results = filterTree.evaluate(*portfolio, boost::none);
			unused(results);
		}

		portfolioCount++;
	}
}

void outputRestrictionResults(
	sic::EvaluationContext &context, std::size_t maxPortfolioCount,
	const sic::ParallelParameters &paraPars = sic::ParallelParameters()) {

	std::unique_ptr<sic::AbstractAssetAllocation::ResultVector>
		results[maxPortfolioCount];

	evaluateRestrictionResultsCore(context, maxPortfolioCount, paraPars,
								   results);

	std::vector<std::thread> threads;
	std::size_t threadCount = paraPars.threadCount;
	std::vector<std::vector<std::string>> globalResultStrings;
	globalResultStrings.resize(threadCount);

	auto serialiseResults = [&](std::size_t threadId,
								std::size_t initialPortfolioIndex,
								std::size_t endPortfolioIndex) {
		std::vector<std::string> &resultStrings =
			globalResultStrings.at(threadId);

		for (std::size_t portfolioIndex = initialPortfolioIndex;
			 portfolioIndex <= endPortfolioIndex; portfolioIndex++) {

			for (auto &resultItem : *results[portfolioIndex]) {
				resultStrings.emplace_back(resultItem->serialise());
			}
		}
	};

	if (paraPars.serial) {
		serialiseResults(0, 0, maxPortfolioCount);
	} else {
		auto portfoliosPerThread = maxPortfolioCount / threadCount;

		for (std::size_t threadID = 0; threadID < threadCount; threadID++) {

			auto initialPortfolioIndex = threadID * portfoliosPerThread;

			// Last thread mops up the remainder.
			auto endPortfolioIndex =
				(threadID < threadCount - 1)
					? initialPortfolioIndex + portfoliosPerThread - 1
					: maxPortfolioCount - 1;

			threads.emplace_back(serialiseResults, threadID,
								 initialPortfolioIndex, endPortfolioIndex);
		}

		for (auto &thread : threads) {
			thread.join();
		}
	}

	unused(globalResultStrings);
}

void evaluateRestrictionResults(
	sic::EvaluationContext &context, std::size_t maxPortfolioCount,
	const sic::ParallelParameters &paraPars = sic::ParallelParameters()) {

	std::unique_ptr<sic::AbstractAssetAllocation::ResultVector>
		results[maxPortfolioCount];

	evaluateRestrictionResultsCore(context, maxPortfolioCount, paraPars,
								   results);

	unused(results);
}

void filterAssets(sic::EvaluationContext &context,
				  std::size_t maxFilterTreeCount, std::size_t maxAssetCount) {
	auto filterTrees = context.getFilterTreeCache().getItems();

	std::size_t filterTreeCount = 0;
	for (const auto &filterTree : context.getFilterTreeCache()) {

		if (filterTreeCount > maxFilterTreeCount) {
			break;
		}

		std::size_t assetCount = 0;
		for (const auto &asset : context.getAssetCache()) {

			if (assetCount > maxAssetCount) {
				break;
			}

			auto &leaf = filterTree->getLeafNode(*asset);
			unused(leaf);

			assetCount++;
		}

		filterTreeCount++;
	}
}

} // namespace UseCase

} // namespace sic

#endif // SIC_USECASES_H_
