#include "sic/AssetRestrictionResult.hh"

#include <sstream>

namespace sic {

std::string AssetRestrictionResult::serialise() const {

	std::ostringstream stringStream;
	stringStream.precision(sic::RestrictionResult::weightDecimalPointPrecision);
	stringStream.flags(std::ostringstream::fixed);

	const auto stateString = std::to_string(static_cast<int>(getState()));
	const auto idString = std::to_string(asset.getExternalID());

	stringStream << toTopWeight;
	const auto toTopWeightStr = stringStream.str();
	stringStream.str("");

	stringStream << weightRange.min;
	const auto minAssetWeightString = stringStream.str();
	stringStream.str("");

	stringStream << weightRange.target;
	const auto targetAssetWeightString = stringStream.str();
	stringStream.str("");

	stringStream << weightRange.max;
	const auto maxAssetWeightString = stringStream.str();
	stringStream.str("");

	return "Asset," + stateString + "," + idString + "," + toTopWeightStr +
		   "," + minAssetWeightString + "," + targetAssetWeightString + "," +
		   maxAssetWeightString + "\n";
}

} // namespace sic
