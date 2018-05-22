#ifndef SIC_MODEL_ASSETALLOCATION_H_
#define SIC_MODEL_ASSETALLOCATION_H_

#include <memory>
#include <unordered_map>

#include "sic/Model/AbstractAssetAllocationNode.hh"
#include "sic/Model/AbstractFilterNode.hh"
#include "sic/Model/AbstractFilterTree.hh"

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

public:
	/// The mapping of filter nodes to AA nodes an AA contains.
	typedef std::unordered_map<const sic::Model::AbstractFilterNode *,
							   const sic::Model::AbstractAssetAllocationNode *>
		FilterNodeMap;

private:
	const sic::Model::AbstractFilterTree &filterTree;
	const std::unique_ptr<sic::Model::AssetAllocation::FilterNodeMap>
		filterNodeMap;

public:
	/**
	 * Construct an AssetAllocation.
	 *
	 * @param filterTree the FilterTree representing the underlying tree
	 * structure of the AssetAllocation.
	 * @param filterNodeMap a mapping of the FilterTree's nodes to the
	 * AssetAllocationNodes where specified.
	 */
	AssetAllocation(const sic::Model::AbstractFilterTree &filterTree,
					std::unique_ptr<sic::Model::AssetAllocation::FilterNodeMap>
						filterNodeMap)
		: filterTree(filterTree), filterNodeMap(std::move(filterNodeMap)) {}

	/**
	 * The FilterTree representing the AA's structure.
	 */
	const sic::Model::AbstractFilterTree &getFilterTree() const {
		return filterTree;
	}
};

} // namespace sic::Model

#endif // SIC_MODEL_ASSETALLOCATION_H_
