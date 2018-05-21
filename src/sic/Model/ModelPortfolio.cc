#include "sic/Model/ModelPortfolio.hh"

#include <stdexcept>

sic::Model::ModelPortfolio::ModelPortfolio(
	std::unique_ptr<sic::Model::ModelPortfolio::AssetWeightMap> assetWeights,
	sic::External::ID externalID)
	: sic::External(externalID) {

	if (assetWeights->empty()) {
		throw std::invalid_argument(
			"assetWeightMap must contain at least one entry.");
	}

	this->assetWeights = std::move(assetWeights);
}

size_t sic::Model::ModelPortfolio::getAssetCount() const {
	return assetWeights->size();
}
