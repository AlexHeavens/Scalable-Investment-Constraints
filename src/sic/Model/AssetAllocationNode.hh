#ifndef SIC_MODEL_ASSETALLOCATION_NODE_H_
#define SIC_MODEL_ASSETALLOCATION_NODE_H_

#include "sic/Base/Types.hh"

namespace sic::Model {

/**
 * A node in an AssetAlloction, mapping a weight range to a filter node.
 */
class AssetAllocationNode {

private:
	const sic::WeightRange weightRange;

public:
	/**
	 * Construct an AssetAllocationNode.
	 */
	AssetAllocationNode(const sic::WeightRange &weightRange)
		: weightRange(weightRange) {}

	/**
	 * The weight range of the node.
	 */
	const sic::WeightRange &getWeightRange() const { return weightRange; }
};

} // namespace sic::Model

#endif // SIC_MODEL_ASSETALLOCATION_NODE_H_
