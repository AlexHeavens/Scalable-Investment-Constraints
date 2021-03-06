#include "sic/Model/AssetAllocation.hh"

#include <stdexcept>

#include "sic/AANodeRestrictionResult.hh"
#include "sic/AssetRestrictionResult.hh"

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

	const auto portfolioValue = portfolio.getTotalReferenceValue();
	std::unordered_map<const sic::AbstractAsset *, sic::Weight>
		portfolioAssetWeights;

	for (const auto &position : portfolio.getPositionIterators()) {
		const auto &asset = position.getAsset();
		const auto positionWeight =
			position.getReferenceValue() / portfolioValue;

		const auto assetLookup = portfolioAssetWeights.find(&asset);
		if (assetLookup != portfolioAssetWeights.end()) {
			assetLookup->second = assetLookup->second + positionWeight;
		} else {
			portfolioAssetWeights.insert(
				std::make_pair(&asset, positionWeight));
		}
	}

	auto modelAssetWeights = getAssetToTopWeights();
	for (const auto &portfolioAssetWeight : portfolioAssetWeights) {

		const sic::AbstractAsset &asset = *(portfolioAssetWeight.first);
		const auto &portfolioWeight = portfolioAssetWeight.second;
		const auto modelAssetWeightLookup = modelAssetWeights->find(&asset);

		if (modelAssetWeightLookup != modelAssetWeights->end()) {
			sic::WeightRange assetModelWeights(modelAssetWeightLookup->second);

			auto assetResult = std::make_unique<sic::AssetRestrictionResult>(
				asset, portfolioWeight, assetModelWeights);
			results->push_back(std::move(assetResult));
		} else {
			sic::WeightRange assetModelWeights(0.0, 0.0, 0.0);

			auto assetResult = std::make_unique<sic::AssetRestrictionResult>(
				asset, portfolioWeight, assetModelWeights);
			results->push_back(std::move(assetResult));
		}
	}

	for (const auto &modelAssetWeightPair : *modelAssetWeights) {
		const auto &asset = *(modelAssetWeightPair.first);

		if (portfolioAssetWeights.find(&asset) == portfolioAssetWeights.end()) {
			const auto &modelAssetWeight = modelAssetWeightPair.second;
			auto assetResult = std::make_unique<sic::AssetRestrictionResult>(
				asset, 0.0, modelAssetWeight);
			results->push_back(std::move(assetResult));
		}
	}

	return results;
}

std::unique_ptr<sic::AbstractAsset::AssetWeightMap>
AssetAllocation::getAssetToTopWeights() const {

	auto assetToTopWeights =
		std::make_unique<sic::AbstractAsset::AssetWeightMap>();

	for (const auto &filterAANodePair : getAANodeIterators()) {

		auto &aaNode = *filterAANodePair.second;
		aaNode.attachToTopAssetWeights(assetToTopWeights.get());
	}

	return assetToTopWeights;
}

} // namespace sic
