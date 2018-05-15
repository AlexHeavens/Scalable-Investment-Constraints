#include "sic/Model/Filter/AssetClassFilter.hh"

bool sic::Model::AssetClassFilter::evaluate(const sic::Asset &asset) const {
	return asset.hasClass(assetClass);
}
