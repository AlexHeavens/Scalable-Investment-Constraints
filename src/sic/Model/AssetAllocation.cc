#include "sic/Model/AssetAllocation.hh"

#include <stdexcept>

namespace sic {

AssetAllocation::AssetAllocation(
	const sic::AbstractFilterTree &filterTree,
	std::unique_ptr<sic::AssetAllocation::FilterNodeMap> filterNodeMap)
	: filterTree(filterTree), filterNodeMap(std::move(filterNodeMap)) {

	if (!this->filterNodeMap or this->filterNodeMap->empty()) {
		throw std::invalid_argument("empty filterNodeMap.");
	}

	for (const auto &filterNodeEntry : *this->filterNodeMap) {
		if (&(filterNodeEntry.first->getFilterTree()) != &filterTree) {
			throw std::invalid_argument(
				"filterNodeMap contains FilterNode from an unknown tree.");
		}
	}
}

} // namespace sic
