#include "sic/Model/ModelPortfolio.hh"

#include <limits>
#include <stdexcept>
#include <unordered_set>

#include "sic/Base/Tolerances.hh"

sic::ModelPortfolio::ModelPortfolio(
	std::unique_ptr<sic::ModelPortfolio::AssetWeightMap> assetWeights,
	sic::External::ID externalID)
	: sic::External(externalID) {

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

	constexpr auto tol = sic::Tolerance<sic::Weight>();
	constexpr auto epsilon = std::numeric_limits<sic::Weight>::epsilon();
	if (sum < (1.0 - tol - epsilon) or sum > (1.0 + tol + epsilon)) {
		throw std::invalid_argument(
			"ModelPortfolio Asset weights must sum to 1.0.");
	}

	this->assetWeights = std::move(assetWeights);
}
sic::Iterators<std::pair<const sic::AbstractAsset *const, sic::WeightRange>>
sic::ModelPortfolio::getAssetWeightIterators() {

	sic::Iterators<
		std::pair<const sic::AbstractAsset *const, sic::WeightRange>>::It
		begin(assetWeights->begin());
	const sic::Iterators<
		std::pair<const sic::AbstractAsset *const, sic::WeightRange>>::It
		end(assetWeights->end());

	return sic::Iterators<
		std::pair<const sic::AbstractAsset *const, sic::WeightRange>>(begin,
																	  end);
}

size_t sic::ModelPortfolio::getAssetCount() const {
	return assetWeights->size();
}
