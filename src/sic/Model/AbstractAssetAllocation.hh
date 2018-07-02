#ifndef SIC_ABSTRACTASSETALLOCATION_H_
#define SIC_ABSTRACTASSETALLOCATION_H_

#include <memory>
#include <unordered_map>

#include "sic/External.hh"
#include "sic/Model/AbstractAssetAllocationNode.hh"
#include "sic/Model/AbstractFilterNode.hh"
#include "sic/Model/AbstractFilterTree.hh"

namespace sic {

/**
 * Interface of an AssetAllocation.
 */
class AbstractAssetAllocation : public sic::External {

public:
	/// The mapping of filter nodes to AA nodes an AA contains.
	using FilterNodeMap = std::unordered_map<
		const sic::AbstractFilterNode *,
		const std::unique_ptr<sic::AbstractAssetAllocationNode>>;

	/**
	 * Construct an AbstractAssetAllocation.
	 *
	 * @param externalID identifier of AA in the external system.
	 * Optional for mock testing.
	 */
	AbstractAssetAllocation(sic::External::ID externalID = 0)
		: sic::External(externalID) {}

	/**
	 * Destroy the AssetAlloction.
	 */
	virtual ~AbstractAssetAllocation() {}

	/**
	 * The FilterTree representing the AA's structure.
	 */
	virtual const sic::AbstractFilterTree &getFilterTree() const = 0;

	/**
	 * Retrieve the AA Node corresponding to a FilterTree node.
	 */
	virtual const sic::AbstractAssetAllocationNode &
	getAANode(const sic::AbstractFilterNode &filterNode) const = 0;

	/**
	 * Iterators for the AssetAllocation's AA nodes.
	 */
	virtual sic::Iterators<FilterNodeMap::value_type>
	getAANodeIterators() const = 0;
};

} // namespace sic

#endif // SIC_ABSTRACTASSETALLOCATION_H_
