#include "sic/AANodeRestrictionResult.hh"

#include <cstdio>

namespace sic {

std::string AANodeRestrictionResult::serialise() const {

	const auto &weightRange = aaNode.getWeightRange();

	// The use of stringstream forces locked access to a common locale object,
	// making it unsuitable for efficient parallel use.  std::to_string also
	// ended up being a time bottleneck. Hence we stick to sprintf, even if it
	// lacks precision specification.

	static const int maxOutputLength = 255;
	char outputString[maxOutputLength];

	std::sprintf(static_cast<char *>(outputString), // NOLINT
				 "AssetAllocationNode,%i,%lu,%f,%f,%f,%f\n",
				 static_cast<int>(getState()), aaNode.getExternalID(),
				 nodeWeight, weightRange.min, weightRange.target,
				 weightRange.max);

	return std::string(static_cast<char *>(outputString));
}

RestrictionResult::State AANodeRestrictionResult::getState() const {
	const auto &nodeWeightRange = aaNode.getWeightRange();
	State state = State::Fail;
	if (nodeWeight >= nodeWeightRange.min and
		nodeWeight <= nodeWeightRange.max) {
		state = State::Pass;
	}
	return state;
}

} // namespace sic
