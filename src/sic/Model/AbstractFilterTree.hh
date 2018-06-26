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
	 * Iterator for the nodes of the tree.
	 *
	 * Searches through the tree in a depth-first manner, starting with the
	 * root.
	 */
	class node_iterator : public std::iterator<std::forward_iterator_tag,
											   const sic::AbstractFilterNode> {
	private:
		const sic::AbstractFilterNode *currentNode;

		/**
		 * The node that the iterator will next choose.
		 */
		const sic::AbstractFilterNode *getNextNode() const;

	public:
		explicit node_iterator(const sic::AbstractFilterNode *currentNode)
			: currentNode(currentNode) {}

		node_iterator &operator++() {
			currentNode = getNextNode();
			return *this;
		}

		node_iterator operator++(int) {
			currentNode = getNextNode();
			return *this;
		}

		bool operator==(node_iterator other) const {
			return currentNode == other.currentNode;
		}

		bool operator!=(node_iterator other) const {
			return currentNode != other.currentNode;
		}

		const sic::AbstractFilterNode &operator*() const {
			return *currentNode;
		}
	};

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

	/**
	 * Begin iterator for the tree nodes.
	 */
	virtual node_iterator begin_nodes() const = 0;

	/**
	 * End iterator for the tree nodes.
	 */
	virtual node_iterator end_nodes() const = 0;

	/**
	 * Begin iterator for the tree nodes.
	 */
	node_iterator begin() const { return begin_nodes(); }

	/**
	 * End iterator for the tree nodes.
	 */
	node_iterator end() const { return end_nodes(); }

	virtual ~AbstractFilterTree() {}
};

} // namespace sic

#endif // SIC-SIC_MODEL_ABSTRACTFILTERTREE_H_
