#ifndef SIC_MODEL_ASSETALLOCATION_H_
#define SIC_MODEL_ASSETALLOCATION_H_

#include "sic/Base/Variable.hh"
#include "sic/Model/AbstractAssetAllocation.hh"

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
private:
	const sic::AbstractFilterTree &filterTree;
	const std::unique_ptr<sic::AssetAllocation::FilterNodeMap> filterNodeMap;
	sic::AbstractAsset::AssetWeightMap modelAssetWeights;

	void getAssetToTopWeights(
		sic::AbstractAsset::AssetWeightMap *assetWeightMap) const;

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

	sic::Iterators<FilterNodeMap::value_type>
	getAANodeIterators() const override {
		return sic::Iterators<FilterNodeMap::value_type>(filterNodeMap);
	};

	std::unique_ptr<ResultVector> generateRestrictionResults(
		const sic::AbstractPortfolio &portfolio) const override;

	std::unique_ptr<sic::AbstractAsset::AssetWeightMap>
	getAssetToTopWeights() const override;
};

} // namespace sic

#endif // SIC_MODEL_ASSETALLOCATION_H_
