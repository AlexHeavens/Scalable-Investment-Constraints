#include "sic/Model/ModelPortfolio.hh"

size_t sic::Model::ModelPortfolio::getAssetCount() const {
	return assetWeights->size();
}
