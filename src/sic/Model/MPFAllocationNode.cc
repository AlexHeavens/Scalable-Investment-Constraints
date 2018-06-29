#include "sic/Model/MPFAllocationNode.hh"

namespace sic {

void MPFAllocationNode::attachToTopAssetWeights(
	sic::AbstractAsset::AssetWeightMap *assetTopWeights) const {

	const auto nodeWeight = getWeightRange().target;
	auto assetIts = mpf.getAssetWeightIterators();
	while (assetIts.remaining()) {

		const auto &assetWeights = *assetIts.current();
		const auto asset = assetWeights.first;
		const auto &weights = assetWeights.second;

		const auto topMinWeight = nodeWeight * weights.min;
		const auto topTargetWeight = nodeWeight * weights.target;
		const auto topMaxWeight = nodeWeight * weights.max;

		sic::WeightRange range(topMinWeight, topTargetWeight, topMaxWeight);
		const std::pair<const sic::AbstractAsset *const, sic::WeightRange> p(
			asset, range);
		assetTopWeights->insert(p);

		assetIts.current()++;
	}
}

} // namespace sic
