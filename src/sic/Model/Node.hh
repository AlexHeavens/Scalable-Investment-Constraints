#ifndef SIC_MODEL_NODE_H_
#define SIC_MODEL_NODE_H_

#include <memory>
#include <vector>

namespace sic::Model {

/**
 * A node in a tree of filters that defines the model (the Asset Allocation).
 *
 * @see sic::Model
 */
class Node {
private:
	const sic::Model::Node *parentNode;
	std::vector<std::unique_ptr<sic::Model::Node>> childNodes;

public:
	/**
	 * Create a root filter tree node.
	 */
	Node() : parentNode(nullptr) {}

	/**
	 * Create a non-root filter tree node.
	 *
	 * @param parentNode the parent node in the filter tree.
	 */
	Node(const sic::Model::Node *parentNode) : parentNode(parentNode) {}

	/**
	 * Add a child node.
	 *
	 * The created node will be owned (and hence, freed) with its parent.
	 *
	 * @return a reference to the created child node.
	 */
	sic::Model::Node &addChild();

	/**
	 * The parent node, if non-root, otherwise nullptr.
	 */
	const sic::Model::Node *getParentNode() const { return parentNode; }
};

} // namespace sic::Model

#endif // SIC_MODEL_NODE_H_
