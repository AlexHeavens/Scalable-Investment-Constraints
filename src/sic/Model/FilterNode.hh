#ifndef SIC_MODEL_FILTERNODE_H_
#define SIC_MODEL_FILTERNODE_H_

#include <memory>
#include <vector>

namespace sic::Model {

/**
 * A FilterNode in a tree of filters that defines the model (the Asset
 * Allocation).
 *
 * @see sic::Model
 */
class FilterNode {
private:
	const sic::Model::FilterNode *parentNode;
	std::vector<std::unique_ptr<sic::Model::FilterNode>> childNodes;

public:
	/**
	 * Create a root filter tree FilterNode.
	 */
	FilterNode() : parentNode(nullptr) {}

	/**
	 * Create a non-root filter tree FilterNode.
	 *
	 * @param parentFilterNode the parent FilterNode in the filter tree.
	 */
	FilterNode(const sic::Model::FilterNode *parentNode)
		: parentNode(parentNode) {}

	/**
	 * Add a child FilterNode.
	 *
	 * The created FilterNode will be owned (and hence, freed) with its parent.
	 *
	 * @return a reference to the created child FilterNode.
	 */
	sic::Model::FilterNode &addChild();

	/**
	 * The parent FilterNode, if non-root, otherwise nullptr.
	 */
	const sic::Model::FilterNode *getParentNode() const { return parentNode; }
};

} // namespace sic::Model

#endif // SIC_MODEL_FILTERNODE_H_
