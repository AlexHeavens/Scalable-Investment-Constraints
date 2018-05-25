#ifndef SIC_MODEL_ABSTRACTASSETALLOCATIONNODE_H_
#define SIC_MODEL_ABSTRACTASSETALLOCATIONNODE_H_

#include "sic/Base/Types.hh"

namespace sic {

/**
 * A node in an abstracted AssetAlloction, mapping a weight range to a filter
 * node.
 *
 * This exists to allow mocking of AssetAllocations in testing.
 *
 * @see sic::AssetAllocation
 */
class AbstractAssetAllocationNode {

public:
	/**
	 * The weight range of the node.
	 */
	virtual const sic::WeightRange &getWeightRange() const = 0;

	virtual ~AbstractAssetAllocationNode() {}
};

} // namespace sic

#endif // SIC_MODEL_ABSTRACTASSETALLOCATION_NODE_H_
