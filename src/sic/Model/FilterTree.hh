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

	node_iterator begin_nodes() const override {
		return node_iterator(&getRootNode());
	}

	node_iterator end_nodes() const override { return node_iterator(nullptr); }
};

} // namespace sic

#endif // SIC_MODEL_FILTERTREE_H_
