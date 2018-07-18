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

void evaluateRestrictionResults(
	sic::EvaluationContext &context, std::size_t maxPortfolioCount,
	const sic::ParallelParameters &paraPars = sic::ParallelParameters()) {

	std::vector<std::thread> threads;
	std::vector<std::string> globalResultsStrings;
	std::mutex resultsMutex;
	std::size_t threadCount = paraPars.threadCount;

	std::function<void(const int)> threadEvaluatePortfolio =
		[&](const std::size_t threadID) {
			std::size_t portfolioCount = 0;
			for (const auto &portfolio : context.getPortfolioCache()) {

				if (portfolioCount > maxPortfolioCount) {
					break;
				}

				if (paraPars.serial) {
					globalResultsStrings.push_back(
						"PortfolioResults," +
						std::to_string(portfolio->getExternalID()) + "\n");

					for (const auto &aa : portfolio->getAssetAllocations()) {

						auto results =
							aa->generateRestrictionResults(*portfolio);

						for (const auto &result : *results) {
							globalResultsStrings.emplace_back(
								result->serialise());
						}
					}
				} else {
					if (portfolioCount % threadCount == threadID) {
						std::vector<std::string> resultStrings;
						resultStrings.push_back(
							"PortfolioResults," +
							std::to_string(portfolio->getExternalID()) + "\n");

						for (const auto &aa :
							 portfolio->getAssetAllocations()) {

							auto results =
								aa->generateRestrictionResults(*portfolio);

							for (const auto &result : *results) {
								resultStrings.emplace_back(result->serialise());
							}
						}

						{
							std::unique_lock<std::mutex> lock(resultsMutex);
							for (const auto &result : resultStrings) {
								globalResultsStrings.push_back(result);
							}
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

	unused(globalResultsStrings);
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
			auto results = filterTree.evaluate(*portfolio);
			unused(results);
		}

		portfolioCount++;
	}
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
