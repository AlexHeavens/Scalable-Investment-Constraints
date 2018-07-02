#include "sic/AANodeRestrictionResult.hh"

#include <sstream>
#include <string>

namespace sic {

std::string AANodeRestrictionResult::serialise() const {
	const auto &nodeWeightRange = aaNode.getWeightRange();

	std::ostringstream stringStream;
	stringStream.precision(sic::RestrictionResult::weightDecimalPointPrecision);
	stringStream.flags(std::ostringstream::fixed);

	const auto stateString = std::to_string(static_cast<int>(getState()));
	const auto idString = std::to_string(aaNode.getExternalID());

	stringStream << nodeWeight;
	const auto nodeWeightString = stringStream.str();
	stringStream.str("");

	stringStream << nodeWeightRange.min;
	const auto minNodeWeightString = stringStream.str();
	stringStream.str("");

	stringStream << nodeWeightRange.target;
	const auto targetNodeWeightString = stringStream.str();
	stringStream.str("");

	stringStream << nodeWeightRange.max;
	const auto maxNodeWeightString = stringStream.str();
	stringStream.str("");

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
