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

static constexpr int fullBankPortfolioCount = 1000000;
static constexpr int rebalancerPortfolioCount = 50000;

void time(const std::string &message, std::function<void()> useCase) {

	auto startTime = std::chrono::high_resolution_clock::now();

	useCase();

	auto finishTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> duration = finishTime - startTime;
	std::cout << message << ", Wall Time, " << duration.count() << "\n";
}

std::unique_ptr<std::vector<std::string>> evaluateRestrictionResults(
	sic::EvaluationContext &context, std::size_t maxPortfolioCount,
	const sic::ParallelParameters &paraPars = sic::ParallelParameters()) {

	std::vector<std::thread> threads;
	auto globalResultsStrings = std::make_unique<std::vector<std::string>>();
	std::mutex resultsMutex;
	std::size_t threadCount = paraPars.threadCount;

	std::function<void(const int)> threadEvaluatePortfolio =
		[&](const std::size_t threadID) {
			time(
				"evaluateRestrictionResults, thread " +
					std::to_string(threadID),
				[&]() {
					std::size_t portfolioCount = 0;
					for (const auto &portfolio : context.getPortfolioCache()) {
						if (portfolioCount > maxPortfolioCount) {
							break;
						}

						if (paraPars.serial) {
							globalResultsStrings->push_back(
								"PortfolioResults," +
								std::to_string(portfolio->getExternalID()) +
								"\n");

							for (const auto &aa :
								 portfolio->getAssetAllocations()) {

								auto results =
									aa->generateRestrictionResults(*portfolio);

								// No need to lock global reuslts, only one
								// thread.
								for (const auto &result : *results) {
									globalResultsStrings->emplace_back(
										result->serialise());
								}
							}
						} else {
							if (portfolioCount % threadCount == threadID) {
								std::vector<std::string> resultStrings;
								resultStrings.push_back(
									"PortfolioResults," +
									std::to_string(portfolio->getExternalID()) +
									"\n");

								for (const auto &aa :
									 portfolio->getAssetAllocations()) {

									auto results =
										aa->generateRestrictionResults(
											*portfolio);

									for (const auto &result : *results) {
										resultStrings.emplace_back(
											result->serialise());
									}
								}

								{
									std::unique_lock<std::mutex> lock(
										resultsMutex);
									for (const auto &result : resultStrings) {
										globalResultsStrings->push_back(result);
									}
								}
							}
						}
						portfolioCount++;
					};
				});
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
	return globalResultsStrings;
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
