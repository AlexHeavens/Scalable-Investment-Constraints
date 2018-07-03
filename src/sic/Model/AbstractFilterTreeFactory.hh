#ifndef SIC_ABSTRACTFILTERTREEFACTORY_H_
#define SIC_ABSTRACTFILTERTREEFACTORY_H_

#include <memory>

#include "sic/Model/AbstractFilterTree.hh"

namespace sic {

/**
 * Interface for objects that generate FilterTrees
 */
class AbstractFilterTreeFactory {

public:
	/**
	 * Construct a FilterTree.
	 */
	virtual std::unique_ptr<sic::AbstractFilterTree>
	create(sic::External::ID externalID) = 0;

	/**
	 * Construct the nodes of a FilterTree on an empty one.
	 */
	virtual void create(sic::AbstractFilterTree &filterTree) = 0;
};

} // namespace sic

#endif // SIC_ABSTRACTFILTERTREEFACTORY_H_
