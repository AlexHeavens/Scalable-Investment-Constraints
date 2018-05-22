#ifndef SIC_MODEL_ASSETALLOCATION_H_
#define SIC_MODEL_ASSETALLOCATION_H_

namespace sic::Model {

/**
 * An Asset Allocation view of clients' target model.
 *
 * An asset allocation defines a heirarchy of nodes that filter and segregate
 * assets of a client's portfolio.  At each node in the tree, the minimum,
 * maximum and target weight of all assets below the node as a proportion of the
 * portfolio value is defined.
 *
 * At leaf nodes in the tree, ModelPortfolios are defined to
 */
class AssetAllocation {

private:
public:
	/**
	 * Construct an AssetAllocation.
	 */
	AssetAllocation() {}
};

} // namespace sic::Model

#endif // SIC_MODEL_ASSETALLOCATION_H_
