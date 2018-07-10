#ifndef SIC_USECASES_H_
#define SIC_USECASES_H_

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

	std::function<void(const int)> threadEvaluatePortfolio =
		[&](const int threadID) {
			int portfolioCount = 0;
			std::vector<std::string> resultStrings;
			for (const auto &portfolio : context.getPortfolioCache()) {

				if (portfolioCount % threadCount == threadID) {
					for (const auto &aa : portfolio->getAssetAllocations()) {
						auto results =
							aa->generateRestrictionResults(*portfolio);
						for (const auto &result : *results) {
							resultStrings.emplace_back(result->serialise());
						}
					}
				}

				portfolioCount++;
			}
			unused(resultStrings);
		};

	for (int threadID = 0; threadID < threadCount; threadID++) {
		threads.emplace_back(threadEvaluatePortfolio, threadID);
	}

	for (auto &thread : threads) {
		thread.join();
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
