#ifndef SIC_MODEL_ABSTRACTFILTERTREE_H_
#define SIC_MODEL_ABSTRACTFILTERTREE_H_

#include "sic/External.hh"

namespace sic {

/**
 * An abstract Tree of Asset filters.]
 *
 * This exists to allow mocking of FilterTrees in testing.
 *
 * @see sic::FilterTree
 */
class AbstractFilterTree : public sic::External {

public:
	/**
	 * Construct a FilterTree.
	 */
	AbstractFilterTree(sic::External::ID externalID)
		: sic::External(externalID) {}
};

} // namespace sic

#endif // SIC-SIC_MODEL_ABSTRACTFILTERTREE_H_
