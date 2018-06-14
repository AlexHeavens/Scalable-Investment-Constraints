#ifndef SIC_MODEL_ABSTRACTFILTERTREE_H_
#define SIC_MODEL_ABSTRACTFILTERTREE_H_

#include "sic/External.hh"
#include "sic/Model/AbstractFilterNode.hh"
#include "sic/Portfolio/AbstractAsset.hh"

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

	/**
	 * Get the root filter node of the tree.
	 */
	virtual sic::AbstractFilterNode &getRootNode() = 0;

	/**
	 * Get the root filter node of the tree.
	 */
	virtual const sic::AbstractFilterNode &getRootNode() const = 0;

	/**
	 * Filter an Asset to its leaf position in the tree.
	 */
	virtual sic::AbstractFilterNode &
	getLeafNode(const sic::AbstractAsset &asset) = 0;

	virtual ~AbstractFilterTree() {}
};

} // namespace sic

#endif // SIC-SIC_MODEL_ABSTRACTFILTERTREE_H_
