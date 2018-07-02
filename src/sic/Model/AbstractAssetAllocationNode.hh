#ifndef SIC_MODEL_ABSTRACTASSETALLOCATIONNODE_H_
#define SIC_MODEL_ABSTRACTASSETALLOCATIONNODE_H_

#include "sic/Base/Types.hh"
#include "sic/External.hh"
#include "sic/Portfolio/AbstractAsset.hh"

namespace sic {

/**
 * A node in an abstracted AssetAlloction, mapping a weight range to a filter
 * node.
 *
 * This exists to allow mocking of AssetAllocations in testing.
 *
 * @see sic::AssetAllocation
 */
class AbstractAssetAllocationNode : public sic::External {

protected:
	/**
	 * Construct a node.
	 *
	 * @param externalID ID of the node in the external system.
	 */
	AbstractAssetAllocationNode(sic::External::ID externalID = 0)
		: sic::External(externalID){};

public:
	/**
	 * The weight range of the node.
	 */
	virtual const sic::WeightRange &getWeightRange() const = 0;

	/**
	 * Add the a set of to-top weights those for Assets directly attached to
	 * this node.
	 */
	virtual void attachToTopAssetWeights(
		sic::AbstractAsset::AssetWeightMap *assetTopWeights) const = 0;

	/**
	 * Destroy the node.
	 */
	virtual ~AbstractAssetAllocationNode() {}
};

} // namespace sic

#endif // SIC_MODEL_ABSTRACTASSETALLOCATION_NODE_H_
