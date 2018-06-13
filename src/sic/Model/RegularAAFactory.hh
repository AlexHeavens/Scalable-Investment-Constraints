#ifndef SIC_REGULARAAFACTORY_HH
#define SIC_REGULARAAFACTORY_HH

#include <memory>

#include "sic/Base/Source.hh"
#include "sic/Model/AbstractAAFactory.hh"
#include "sic/Model/AbstractFilterTree.hh"

namespace sic {

class RegularAAFactory : public sic::AbstractAAFactory {

private:
	sic::Source<std::unique_ptr<sic::AbstractFilterTree>> &filterTreeSource;
	sic::Iterators<std::unique_ptr<sic::AbstractFilterTree>> filterTrees;

public:
	RegularAAFactory(
		sic::Source<std::unique_ptr<sic::AbstractFilterTree>> &filterTreeSource)
		: filterTreeSource(filterTreeSource),
		  filterTrees(filterTreeSource.getItems()) {}

	std::unique_ptr<sic::AbstractAssetAllocation> create() override;
};

} // namespace sic

#endif // SIC_REGULARAAFACTORY_HH
