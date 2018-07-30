#include "sic/AANodeRestrictionResult.hh"

namespace sic {

std::string AANodeRestrictionResult::serialise() const {

	const auto &nodeWeightRange = aaNode.getWeightRange();

	// The use of stringstream forces locked access to a common locale object,
	// making it unsuitable for efficient parallel use.  Hence we stick to
	// simple std::to_string, even if it lacks precision specification.
	const auto stateString = std::to_string(static_cast<int>(getState()));
	const auto idString = std::to_string(aaNode.getExternalID());

	const auto nodeWeightString = std::to_string(nodeWeight);
	const auto minNodeWeightString = std::to_string(nodeWeightRange.min);
	const auto targetNodeWeightString = std::to_string(nodeWeightRange.target);
	const auto maxNodeWeightString = std::to_string(nodeWeightRange.max);

	return "AssetAllocationNode," + stateString + "," + idString + "," +
		   nodeWeightString + "," + minNodeWeightString + "," +
		   targetNodeWeightString + "," + maxNodeWeightString + "\n";
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
