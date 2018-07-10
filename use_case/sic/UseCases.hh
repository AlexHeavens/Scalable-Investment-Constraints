#ifndef SIC_USECASES_H_
#define SIC_USECASES_H_

#include "sic/Base/Variable.hh"
#include "sic/EvaluationContext.hh"

namespace sic {

namespace UseCase {

void evaluateRestrictionResults(sic::EvaluationContext &context) {

	std::vector<std::string> resultStrings;
	for (const auto &portfolio : context.getPortfolioCache()) {
		for (const auto &aa : portfolio->getAssetAllocations()) {
			const auto &filterTree = aa->getFilterTree();
			auto results = filterTree.evaluate(*portfolio);
			unused(results);
		}
	}
	unused(resultStrings);
}

void evaluatePortfolios(sic::EvaluationContext &context) {

	std::vector<std::string> resultStrings;
	for (const auto &portfolio : context.getPortfolioCache()) {
		for (const auto &aa : portfolio->getAssetAllocations()) {
			auto results = aa->generateRestrictionResults(*portfolio);
			for (const auto &result : *results) {
				resultStrings.emplace_back(result->serialise());
			}
		}
	}
	unused(resultStrings);
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
