#ifndef SIC_USECASES_H_
#define SIC_USECASES_H_

#include <mutex>
#include <thread>

#include "sic/Base/Variable.hh"
#include "sic/EvaluationContext.hh"

namespace sic {

namespace UseCase {

void evaluateRestrictionResults(sic::EvaluationContext &context) {

	int threadCount = std::thread::hardware_concurrency();

	// Zero implies thread count is unknown.
	if (threadCount == 0) {
		threadCount = 1;
	}

	std::vector<std::thread> threads;
	std::vector<std::string> results;
	std::mutex resultsMutex;

	std::function<void(const int)> threadEvaluatePortfolio =
		[&](const int threadID) {
			int portfolioCount = 0;
			for (const auto &portfolio : context.getPortfolioCache()) {

				if (portfolioCount % threadCount == threadID) {
					std::vector<std::string> resultStrings;
					resultStrings.push_back(
						"PortfolioResults," +
						std::to_string(portfolio->getExternalID()) + "\n");

					for (const auto &aa : portfolio->getAssetAllocations()) {

						auto results =
							aa->generateRestrictionResults(*portfolio);
						for (const auto &result : *results) {
							resultStrings.emplace_back(result->serialise());
						}
					}

					resultsMutex.lock();
					for (const auto &result : resultStrings) {
						results.push_back(result);
					}
					resultsMutex.unlock();
				}

				portfolioCount++;
			}
		};

	for (int threadID = 0; threadID < threadCount; threadID++) {
		threads.emplace_back(threadEvaluatePortfolio, threadID);
	}

	for (auto &thread : threads) {
		thread.join();
	}

	unused(results);
}

void evaluatePortfolios(sic::EvaluationContext &context) {

	for (const auto &portfolio : context.getPortfolioCache()) {
		for (const auto &aa : portfolio->getAssetAllocations()) {
			const auto &filterTree = aa->getFilterTree();
			auto results = filterTree.evaluate(*portfolio);
			unused(results);
		}
	}
}

void filterAssets(sic::EvaluationContext &context) {
	auto filterTrees = context.getFilterTreeCache().getItems();

	for (const auto &filterTree : context.getFilterTreeCache()) {

		for (const auto &asset : context.getAssetCache()) {

			auto &leaf = filterTree->getLeafNode(*asset);
			unused(leaf);
		}
	}
}

} // namespace UseCase

} // namespace sic

#endif // SIC_USECASES_H_
