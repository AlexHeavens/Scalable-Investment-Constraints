#ifndef SIC_MODEL_FILTERTREE_H_
#define SIC_MODEL_FILTERTREE_H_

#include "sic/Model/AbstractFilterTree.hh"
#include "sic/Model/FilterNode.hh"

namespace sic {

/**
 * A Tree of Asset filters.
 */
class FilterTree : public sic::AbstractFilterTree {

public:
	typedef std::vector<const sic::AbstractFilterNode *> Path;

private:
	sic::FilterNode rootNode;

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
		sic::AbstractFilterNode *currentNode;

		/**
		 * The node that the iterator will next choose.
		 */
		sic::AbstractFilterNode *getNextNode() const;

	public:
		explicit node_iterator(sic::AbstractFilterNode *currentNode)
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

		sic::AbstractFilterNode &operator*() const { return *currentNode; }
	};

	/**
	 * Construct a FilterTree.
	 */
	FilterTree(sic::External::ID externalID)
		: sic::AbstractFilterTree(externalID), rootNode(*this, 0) {}

	sic::AbstractFilterNode &getRootNode() override { return rootNode; }

	const sic::AbstractFilterNode &getRootNode() const override {
		return rootNode;
	}

	/**
	 * Get a path of an Asset through the FilterTree.
	 *
	 * @param asset Asset to find path of.
	 * @param path Path container to append the list of nodes in the path.
	 */
	void getAssetPath(const sic::AbstractAsset &asset,
					  sic::FilterTree::Path &path) const;

	const sic::AbstractFilterNode &
	getLeafNode(const sic::AbstractAsset &asset) const override;

	std::unique_ptr<sic::AbstractValueTree>
	evaluate(const sic::AbstractPortfolio &portfolio) const override;

	/**
	 * Begin iterator for the tree nodes.
	 */
	node_iterator begin_nodes() { return node_iterator(&getRootNode()); }

	/**
	 * End iterator for the tree nodes.
	 */
	node_iterator end_nodes() { return node_iterator(nullptr); }
};

} // namespace sic

#endif // SIC_MODEL_FILTERTREE_H_
