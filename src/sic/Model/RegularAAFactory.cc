#include "sic/Model/RegularAAFactory.hh"

#include "sic/Model/AssetAllocation.hh"

namespace sic {

std::unique_ptr<sic::AbstractAssetAllocation> sic::RegularAAFactory::create() {

	// Take FilterTrees in a cyclic manner from the source.
	if (!filterTrees.remaining()) {
		filterTrees = filterTreeSource.getItems();
	}

	const auto &filterTree = **filterTrees.current();
	filterTrees.current()++;

	std::unique_ptr<sic::AssetAllocation::FilterNodeMap> filterNodeMap;
	sic::AssetAllocation *newAA =
		new sic::AssetAllocation(filterTree, std::move(filterNodeMap));

	std::unique_ptr<sic::AbstractAssetAllocation> newAAPtr(newAA);

	return newAAPtr;
}

} // namespace sic
