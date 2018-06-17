#ifndef SIC_MODEL_ABSTRACTFILTERTREE_H_
#define SIC_MODEL_ABSTRACTFILTERTREE_H_

#include <memory>

#include "sic/AbstractPortfolio.hh"
#include "sic/External.hh"
#include "sic/Model/AbstractFilterNode.hh"
#include "sic/Portfolio/AbstractAsset.hh"
#include "sic/Portfolio/AbstractValueTree.hh"

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
	virtual const sic::AbstractFilterNode &
	getLeafNode(const sic::AbstractAsset &asset) const = 0;

	/**
	 * Produce a value breakdown of a Portfolio against the tree's nodes.
	 *
	 * @param portfolio client's portfolio.
	 * @return a mapping of the tree's grouping nodes to the weight of
	 * portfolio's assets that fall under that node.
	 */
	virtual std::unique_ptr<sic::AbstractValueTree>
	evaluate(const sic::AbstractPortfolio &portfolio) const = 0;

	virtual ~AbstractFilterTree() {}
};

} // namespace sic

#endif // SIC-SIC_MODEL_ABSTRACTFILTERTREE_H_
