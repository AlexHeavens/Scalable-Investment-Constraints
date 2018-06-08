#ifndef SIC_MODEL_ABSTRACTFILTERNODE_H_
#define SIC_MODEL_ABSTRACTFILTERNODE_H_

#include <vector>

#include "sic/Base/Iterators.hh"
#include "sic/Model/Filter.hh"
#include "sic/Portfolio/AbstractAsset.hh"

namespace sic {

/**
 * A FilterNode in a tree of filters that defines the model (the Asset
 * Allocation).
 *
 * @see sic
 */
class AbstractFilterNode {

public:
	typedef std::vector<std::unique_ptr<sic::AbstractFilterNode>>
		ChildNodeVector;
	typedef sic::AbstractFilterNode::ChildNodeVector::iterator ChildIterator;
	typedef sic::AbstractFilterNode::ChildNodeVector::const_iterator
		ConstChildIterator;

	/**
	 * Add a child FilterNode.
	 *
	 * The created FilterNode will be owned (and hence, freed) with its parent.
	 *
	 * @param childFilter the Filter that Assets must pass to fall under the
	 * child node.  The child node will take ownership of this filter.
	 * @return a reference to the created child FilterNode.
	 */
	virtual sic::AbstractFilterNode &
	addChild(std::unique_ptr<const sic::Filter> childFilter) = 0;

	/**
	 * The parent FilterNode, if non-root, otherwise nullptr.
	 */
	virtual const sic::AbstractFilterNode *getParentNode() const = 0;

	/**
	 * Get the filter stored.
	 */
	virtual const sic::Filter &getFilter() const = 0;

	/**
	 * Return the child that matches a given Asset.
	 *
	 * If no child matches, nullptr is returned.
	 */
	virtual sic::AbstractFilterNode *
	filterToChild(const sic::AbstractAsset &asset) = 0;

	/**
	 * The current and end iterators of the node's children.
	 *
	 * @return Current and end iterators.
	 */
	virtual sic::Iterators<sic::AbstractFilterNode::ChildIterator>
	getChildIterators() = 0;

	/**
	 * The current and end iterators of the node's children.
	 *
	 * @return Current and end iterators.
	 */
	virtual sic::Iterators<sic::AbstractFilterNode::ConstChildIterator>
	getChildIterators() const = 0;

	/**
	 * The number of children below the node.
	 */
	virtual std::size_t getChildCount() const = 0;

	virtual ~AbstractFilterNode(){};
};

} // namespace sic

#endif // SIC_MODEL_ABSTRACTFILTERNODE_H_
