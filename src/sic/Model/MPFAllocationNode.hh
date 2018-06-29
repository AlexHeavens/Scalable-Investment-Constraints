#ifndef SIC_MODEL_MPFALLOCATIONNODE_H_
#define SIC_MODEL_MPFALLOCATIONNODE_H_

#include "sic/Model/AbstractModelPortfolio.hh"
#include "sic/Model/AssetAllocationNode.hh"

namespace sic {

/**
 * An AssetAllocation node that additionally requires the resulting Assets to
 * follow an ModelPortfolio.
 */
class MPFAllocationNode : public sic::AssetAllocationNode {

private:
	const sic::AbstractModelPortfolio &mpf;

public:
	/**
	 * Create an MPFAllocationNode.
	 *
	 * @param mpf MPF Assets that fall into this node must follow.
	 * @param weightRange range of the summed weights that Assets in this node
	 * must follow.
	 */
	MPFAllocationNode(const sic::AbstractModelPortfolio &mpf,
					  const sic::WeightRange &weightRange)
		: sic::AssetAllocationNode(weightRange), mpf(mpf) {}

	/**
	 * The ModelPortfolio the node follows.
	 */
	const sic::AbstractModelPortfolio &getModelPortfolio() const { return mpf; }

	void attachToTopAssetWeights(
		sic::AbstractAsset::AssetWeightMap *assetTopWeights) const override;
};

} // namespace sic

#endif // SIC_MODEL_MPFALLOCATIONNODE_H_
