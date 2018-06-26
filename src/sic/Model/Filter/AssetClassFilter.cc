#include "sic/Model/Filter/AssetClassFilter.hh"

namespace sic {

bool AssetClassFilter::evaluate(const sic::AbstractAsset &asset) const {
	return asset.hasClass(assetClass);
}

} // namespace sic
