#include "sic/Model/ModelPortfolio.hh"

#include <stdexcept>
#include <unordered_set>

sic::Model::ModelPortfolio::ModelPortfolio(
	std::unique_ptr<sic::Model::ModelPortfolio::AssetWeightMap> assetWeights,
	sic::External::ID externalID)
	: sic::External(externalID) {

	if (assetWeights->empty()) {
		throw std::invalid_argument(
			"assetWeightMap must contain at least one entry.");
	}

	std::unordered_set<sic::External::ID> inputAssetIDs;
	for (const auto &assetEntry : *assetWeights) {
		const auto assetID = assetEntry.first->getExternalID();
		if (inputAssetIDs.find(assetID) != inputAssetIDs.end()) {
			throw std::invalid_argument(
				"assetWeightMap must not contain duplicate Assets.");
		}

		inputAssetIDs.insert(assetID);
	}

	this->assetWeights = std::move(assetWeights);
}

size_t sic::Model::ModelPortfolio::getAssetCount() const {
	return assetWeights->size();
}
