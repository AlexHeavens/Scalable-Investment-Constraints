#ifndef SIC_AANODERESTRICTIONRESULT_H_
#define SIC_AANODERESTRICTIONRESULT_H_

#include "sic/Model/AbstractAssetAllocationNode.hh"
#include "sic/RestrictionResult.hh"

namespace sic {

/**
 * The result of evaluate the bounds on an AA node.
 */
class AANodeRestrictionResult : public sic::RestrictionResult {

private:
	const sic::AbstractAssetAllocationNode &aaNode;
	const sic::Weight nodeWeight;

public:
	/**
	 * Construct the result.
	 */
	AANodeRestrictionResult(const sic::AbstractAssetAllocationNode &aaNode,
							sic::Weight nodeWeight)
		: aaNode(aaNode), nodeWeight(nodeWeight) {}

	std::string serialise() const override;

	State getState() const override;

	bool operator==(const sic::RestrictionResult &other) const override {
		const sic::AANodeRestrictionResult *otherCast =
			dynamic_cast<const sic::AANodeRestrictionResult *>(&other);
		bool match = false;
		if (otherCast != nullptr) {
			match = &aaNode == &otherCast->aaNode and
					nodeWeight == otherCast->nodeWeight;
		}
		return match;
	}
};

} // namespace sic

#endif // SIC_AANODERESTRICTIONRESULT_H_
