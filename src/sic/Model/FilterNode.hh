#ifndef SIC_MODEL_FILTERNODE_H_
#define SIC_MODEL_FILTERNODE_H_

#include <memory>
#include <vector>

#include "sic/Model/AbstractFilterNode.hh"

namespace sic::Model {

/**
 * A FilterNode in a tree of filters that defines the model (the Asset
 * Allocation).
 *
 * @see sic::Model
 */
class FilterNode : public sic::Model::AbstractFilterNode {
private:
	const sic::Model::AbstractFilterNode *parentNode;
	std::vector<std::unique_ptr<sic::Model::AbstractFilterNode>> childNodes;

	/**
	 * Create a non-root filter tree FilterNode.
	 *
	 * This is private to ensure the consistency of the parentNode pointer.
	 *
	 * @param parentFilterNode the parent FilterNode in the filter tree.
	 */
	FilterNode(const sic::Model::AbstractFilterNode *parentNode)
		: parentNode(parentNode) {}

public:
	/**
	 * Create a root filter tree FilterNode.
	 */
	FilterNode() : parentNode(nullptr) {}

	~FilterNode() override {}

	sic::Model::AbstractFilterNode &addChild() override;

	const sic::Model::AbstractFilterNode *getParentNode() const override {
		return parentNode;
	}
};

} // namespace sic::Model

#endif // SIC_MODEL_FILTERNODE_H_
