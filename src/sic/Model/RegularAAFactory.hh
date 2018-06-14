#ifndef SIC_REGULARAAFACTORY_HH
#define SIC_REGULARAAFACTORY_HH

#include <memory>

#include "sic/Base/Source.hh"
#include "sic/Model/AbstractAAFactory.hh"
#include "sic/Model/AbstractFilterTree.hh"
#include "sic/Model/AssetAllocation.hh"

namespace sic {

/**
 * Factory for creating AssetAllocations with regular FilterTree structures and
 * equally distributed node target weights.
 */
class RegularAAFactory : public sic::AbstractAAFactory {

private:
	sic::Source<std::unique_ptr<sic::AbstractFilterTree>> &filterTreeSource;
	sic::Iterators<std::unique_ptr<sic::AbstractFilterTree>> filterTrees;
	sic::External::ID nextAAID;

	static void addAANodes(const sic::AbstractFilterNode &node,
						   sic::Weight parentWeight,
						   sic::AssetAllocation::FilterNodeMap *filterNodeMap);

public:
	/**
	 * Construct a Factory.
	 *
	 * @param filterTreeSource source of FilterTrees.  These will be given to
	 * the generated AAs in a round-robin fashion.
	 * @param initialAAID ID from which AA IDs will start counting.
	 */
	RegularAAFactory(
		sic::Source<std::unique_ptr<sic::AbstractFilterTree>> &filterTreeSource,
		sic::External::ID initialAAID = 0)
		: filterTreeSource(filterTreeSource),
		  filterTrees(filterTreeSource.getItems()), nextAAID(initialAAID) {}

	std::unique_ptr<sic::AbstractAssetAllocation> create() override;
};

} // namespace sic

#endif // SIC_REGULARAAFACTORY_HH
