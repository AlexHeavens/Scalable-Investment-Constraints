#include "sic/Model/ModelPortfolio.hh"

#include <stdexcept>
#include <unordered_set>

#include "sic/Base/Tolerances.hh"

namespace sic {

ModelPortfolio::ModelPortfolio(
	std::unique_ptr<sic::AbstractAsset::AssetWeightMap> assetWeights,
	sic::External::ID externalID)
	: sic::AbstractModelPortfolio(externalID) {

	if (assetWeights->empty()) {
		throw std::invalid_argument(
			"assetWeightMap must contain at least one entry.");
	}

	sic::Weight sum = 0.0;
	std::unordered_set<sic::External::ID> inputAssetIDs;
	for (const auto &assetEntry : *assetWeights) {

		const auto assetID = (*assetEntry.first).getExternalID();
		if (inputAssetIDs.find(assetID) != inputAssetIDs.end()) {
			throw std::invalid_argument(
				"assetWeightMap must not contain duplicate Assets.");
		}

		inputAssetIDs.insert(assetID);
		sum += assetEntry.second.target;
	}

	// Check within tolerance of acceptable rounding errors.
	constexpr auto tol = sic::Tolerance<sic::Weight>();
	constexpr auto minSum = 1.0 - tol;
	constexpr auto maxSum = 1.0 + tol;

	if (sum < minSum or sum > maxSum) {
		throw std::invalid_argument(
			"ModelPortfolio Asset weights must sum to 1.0.");
	}

	this->assetWeights = std::move(assetWeights);
}

sic::Iterators<std::pair<const sic::AbstractAsset *const, sic::WeightRange>>
ModelPortfolio::getAssetWeightIterators() const {
	return sic::Iterators<
		std::pair<const sic::AbstractAsset *const, sic::WeightRange>>(
		assetWeights);
}

} // namespace sic
