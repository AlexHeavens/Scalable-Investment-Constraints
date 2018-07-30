#include "sic/AssetRestrictionResult.hh"

namespace sic {

std::string AssetRestrictionResult::serialise() const {

	// The use of stringstream forces locked access to a common locale object,
	// making it unsuitable for efficient parallel use.  Hence we stick to
	// simple std::to_string, even if it lacks precision specification.
	const auto stateString = std::to_string(static_cast<int>(getState()));
	const auto idString = std::to_string(asset.getExternalID());

	const auto toTopWeightStr = std::to_string(toTopWeight);
	const auto minAssetWeightString = std::to_string(weightRange.min);
	const auto targetAssetWeightString = std::to_string(weightRange.target);
	const auto maxAssetWeightString = std::to_string(weightRange.max);

	return "Asset," + stateString + "," + idString + "," + toTopWeightStr +
		   "," + minAssetWeightString + "," + targetAssetWeightString + "," +
		   maxAssetWeightString + "\n";
}

} // namespace sic
