#include "sic/Model/AssetAllocation.hh"

#include <stdexcept>

#include "sic/AANodeRestrictionResult.hh"

namespace sic {

AssetAllocation::AssetAllocation(
	const sic::AbstractFilterTree &filterTree,
	std::unique_ptr<sic::AssetAllocation::FilterNodeMap> filterNodeMap,
	sic::External::ID externalID)
	: sic::AbstractAssetAllocation(externalID), filterTree(filterTree),
	  filterNodeMap(std::move(filterNodeMap)) {

	if (!this->filterNodeMap or this->filterNodeMap->empty()) {
		throw std::invalid_argument("empty filterNodeMap.");
	}

	for (const auto &filterNodeEntry : *this->filterNodeMap) {
		if (&(filterNodeEntry.first->getFilterTree()) != &filterTree) {
			throw std::invalid_argument(
				"filterNodeMap contains FilterNode from an unknown tree.");
		}
	}
}

std::unique_ptr<AbstractAssetAllocation::ResultVector>
AssetAllocation::generateRestrictionResults(
	const sic::AbstractPortfolio &portfolio) const {

	auto results = std::make_unique<ResultVector>();

	auto valueTree = filterTree.evaluate(portfolio);

	auto nodeWeightIts = valueTree->getNodeWeightIterators();
	while (nodeWeightIts.remaining()) {

		const auto &nodeWeightPair = *nodeWeightIts.current();
		const auto &filterNode = *nodeWeightPair.first;
		const auto &nodeWeight = nodeWeightPair.second;

		const auto &nodeMapPair = *filterNodeMap->find(&filterNode);
		const auto &aaNode = *nodeMapPair.second;

		std::unique_ptr<sic::RestrictionResult> result(
			new sic::AANodeRestrictionResult(aaNode, nodeWeight));
		results->push_back(std::move(result));

		nodeWeightIts.current()++;
	}

	return results;
}

} // namespace sic
