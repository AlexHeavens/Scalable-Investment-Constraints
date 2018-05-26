#ifndef SIC_ABSTRACTFILTERTREEFACTORY_H_
#define SIC_ABSTRACTFILTERTREEFACTORY_H_

#include <memory>

#include "sic/Model/FilterTree.hh"

namespace sic {

/**
 * Interface for objects that generate FilterTrees
 */
class AbstractFilterTreeFactory {

public:
	virtual std::unique_ptr<sic::FilterTree>
	create(sic::External::ID externalID) = 0;
};

} // namespace sic

#endif // SIC_ABSTRACTFILTERTREEFACTORY_H_
