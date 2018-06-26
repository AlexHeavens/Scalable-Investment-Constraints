#ifndef SIC_REGULARAAFACTORY_HH
#define SIC_REGULARAAFACTORY_HH

#include <memory>

#include "sic/Base/Source.hh"
#include "sic/Model/AbstractAAFactory.hh"
#include "sic/Model/AbstractFilterTree.hh"
#include "sic/Model/AbstractModelPortfolio.hh"
#include "sic/Model/AssetAllocation.hh"

namespace sic {

/**
 * Factory for creating AssetAllocations with regular FilterTree structures and
 * equally distributed node target weights.
 */
class RegularAAFactory : public sic::AbstractAAFactory {

public:
	using NodeMPFMap = std::unordered_map<const sic::AbstractFilterNode *,
										  const sic::AbstractModelPortfolio *>;
	using NodeAssets =
		std::unordered_map<const sic::AbstractFilterNode *,
						   std::vector<const sic::AbstractAsset *>>;

	/// Percentage of the target that the min / max will differ by.
	static constexpr sic::Weight TARGET_WEIGHT_TOL = 0.1;

private:
	sic::Source<std::unique_ptr<sic::AbstractFilterTree>> &filterTreeSource;
	sic::Iterators<std::unique_ptr<sic::AbstractFilterTree>> filterTrees;
	const std::vector<std::unique_ptr<sic::AbstractAsset>> &assetSource;
	const size_t mpfAssetCount;
	sic::External::ID nextAAID, nextMPFID;

	void genLeafNodeAssetMap(const sic::AbstractFilterTree &filterTree,
							 NodeAssets *leafNodeAssets) const;

	void
	genMPFs(const NodeAssets &leafNodeAssets, NodeMPFMap *nodeMPFMap,
			std::vector<std::unique_ptr<sic::AbstractModelPortfolio>> *newMPFs);

	void genAANodes(const sic::AbstractFilterTree &filterTree,
					const NodeMPFMap &nodeMPFMap,
					sic::AssetAllocation::FilterNodeMap *filterNodeMap) const;

	void
	genChildAANodes(const sic::AbstractFilterNode &node,
					const sic::Weight parentWeight,
					const NodeMPFMap &nodeMPFMap,
					sic::AssetAllocation::FilterNodeMap *filterNodeMap) const;

public:
	/**
	 * Construct a Factory.
	 *
	 * @param filterTreeSource source of FilterTrees.  These will be given to
	 * the generated AAs in a round-robin fashion.
	 * @param assetSource assets to randomly assign to the generated MPFs.
	 * @param mpfAssetCount assets added to each created MPF.
	 * @param initialAAID ID from which AA IDs will be assigned.
	 * @param initialMPFID ID from which MPF IDs will be assigned.
	 */
	RegularAAFactory(
		sic::Source<std::unique_ptr<sic::AbstractFilterTree>> &filterTreeSource,
		const std::vector<std::unique_ptr<sic::AbstractAsset>> &assetSource,
		const std::size_t mpfAssetCount, sic::External::ID initialAAID = 0,
		sic::External::ID initialMPFID = 0);

	Result create() override;
}; // namespace sic

} // namespace sic

#endif // SIC_REGULARAAFACTORY_HH
