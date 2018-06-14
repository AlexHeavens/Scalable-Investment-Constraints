#ifndef SIC_MODEL_ASSETALLOCATION_H_
#define SIC_MODEL_ASSETALLOCATION_H_

#include <memory>
#include <unordered_map>

#include "sic/Model/AbstractAssetAllocation.hh"
#include "sic/Model/AbstractAssetAllocationNode.hh"
#include "sic/Model/AbstractFilterNode.hh"

namespace sic {

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
class AssetAllocation : public sic::AbstractAssetAllocation {

public:
	/// The mapping of filter nodes to AA nodes an AA contains.
	typedef std::unordered_map<
		const sic::AbstractFilterNode *,
		const std::unique_ptr<sic::AbstractAssetAllocationNode>>
		FilterNodeMap;

private:
	const sic::AbstractFilterTree &filterTree;
	const std::unique_ptr<sic::AssetAllocation::FilterNodeMap> filterNodeMap;

public:
	/**
	 * Construct an AssetAllocation.
	 *
	 * @param filterTree the FilterTree representing the underlying tree
	 * structure of the AssetAllocation.
	 * @param filterNodeMap a mapping of the FilterTree's nodes to the
	 * AssetAllocationNodes where specified.  Must be non-empty.
	 * @param externalID ID of the AA in the external system.
	 */
	AssetAllocation(
		const sic::AbstractFilterTree &filterTree,
		std::unique_ptr<sic::AssetAllocation::FilterNodeMap> filterNodeMap,
		sic::External::ID externalID);

	const sic::AbstractFilterTree &getFilterTree() const override {
		return filterTree;
	}

	const sic::AbstractAssetAllocationNode &
	getAANode(const sic::AbstractFilterNode &filterNode) const override {
		return *(filterNodeMap->at(&filterNode));
	};
};

} // namespace sic

#endif // SIC_MODEL_ASSETALLOCATION_H_
