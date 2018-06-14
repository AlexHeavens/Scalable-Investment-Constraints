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

	static void addAANodes(const sic::AbstractFilterNode &node,
						   sic::Weight parentWeight,
						   sic::AssetAllocation::FilterNodeMap *filterNodeMap);

public:
	/**
	 * Construct a Factory.
	 *
	 * @param filterTreeSource source of FilterTrees.  These will be given to
	 * the generated AAs in a round-robin fashion.
	 */
	RegularAAFactory(
		sic::Source<std::unique_ptr<sic::AbstractFilterTree>> &filterTreeSource)
		: filterTreeSource(filterTreeSource),
		  filterTrees(filterTreeSource.getItems()) {}

	std::unique_ptr<sic::AbstractAssetAllocation> create() override;
};

} // namespace sic

#endif // SIC_REGULARAAFACTORY_HH
