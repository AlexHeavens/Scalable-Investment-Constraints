#include "sic/AssetRestrictionResult.hh"

#include <cstdio>

namespace sic {

std::string AssetRestrictionResult::serialise() const {

	// The use of stringstream forces locked access to a common locale object,
	// making it unsuitable for efficient parallel use.  std::to_string also
	// ended up being a time bottleneck. Hence we stick to sprintf, even if it
	// lacks precision specification.

	static const int maxOutputLength = 255;
	char outputString[maxOutputLength];

	std::sprintf(static_cast<char *>(outputString), // NOLINT
				 "Asset,%i,%lu,%f,%f,%f,%f\n", static_cast<int>(getState()),
				 asset.getExternalID(), toTopWeight, weightRange.min,
				 weightRange.target, weightRange.max);

	return std::string(static_cast<char *>(outputString));
}

} // namespace sic
