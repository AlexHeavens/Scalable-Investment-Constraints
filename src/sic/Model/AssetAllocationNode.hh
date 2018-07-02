#ifndef SIC_MODEL_ASSETALLOCATION_NODE_H_
#define SIC_MODEL_ASSETALLOCATION_NODE_H_

#include "sic/Base/Types.hh"
#include "sic/Base/Variable.hh"
#include "sic/Model/AbstractAssetAllocationNode.hh"

namespace sic {

/**
 * A node in an AssetAlloction, mapping a weight range to a filter node.
 */
class AssetAllocationNode : public sic::AbstractAssetAllocationNode {

private:
	const sic::WeightRange weightRange;

public:
	/**
	 * Construct an AssetAllocationNode.
	 *
	 * @param weightRange range of the summed weights that Assets in this node
	 * must follow.
	 * @param externalID ID of the node in the external system.
	 */
	AssetAllocationNode(const sic::WeightRange &weightRange,
						sic::External::ID externalID)
		: sic::AbstractAssetAllocationNode(externalID),
		  weightRange(weightRange){};

	/**
	 * The weight range of the node.
	 */
	const sic::WeightRange &getWeightRange() const override {
		return weightRange;
	}

	void attachToTopAssetWeights(
		sic::AbstractAsset::AssetWeightMap *assetTopWeights) const override {

		// Most nodes do not have directly attached Assets.
		unused(assetTopWeights)
	}
};

} // namespace sic

#endif // SIC_MODEL_ASSETALLOCATION_NODE_H_
