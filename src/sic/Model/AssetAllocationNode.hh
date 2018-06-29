#ifndef SIC_MODEL_ASSETALLOCATION_NODE_H_
#define SIC_MODEL_ASSETALLOCATION_NODE_H_

#include "sic/Base/Types.hh"
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
	 */
	AssetAllocationNode(const sic::WeightRange &weightRange)
		: weightRange(weightRange) {}

	/**
	 * The weight range of the node.
	 */
	const sic::WeightRange &getWeightRange() const override {
		return weightRange;
	}

	void attachToTopAssetWeights(
		sic::AbstractAsset::AssetWeightMap *assetTopWeights) const override {

		// Most nodes do not have directly attached Assets.
#pragma unused(assetTopWeights)
	}
};

} // namespace sic

#endif // SIC_MODEL_ASSETALLOCATION_NODE_H_
