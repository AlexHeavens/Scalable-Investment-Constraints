#ifndef SIC_MODEL_FILTERNODE_H_
#define SIC_MODEL_FILTERNODE_H_

#include <memory>

#include "sic/Model/AbstractFilterNode.hh"
#include "sic/Model/Filter.hh"
#include "sic/Model/Filter/AllAssetsFilter.hh"

namespace sic {

/**
 * A FilterNode in a tree of filters that defines the model (the Asset
 * Allocation).
 *
 * @see sic
 */
class FilterNode : public sic::AbstractFilterNode {

private:
	const sic::AbstractFilterNode *parentNode;
	sic::AbstractFilterNode::ChildNodeVector childNodes;

	const std::unique_ptr<const sic::Filter> filter;

	/**
	 * Create a non-root filter tree FilterNode.
	 *
	 * This is private to ensure the consistency of the parentNode pointer.
	 *
	 * @param parentFilterNode the parent FilterNode in the filter tree.
	 */
	FilterNode(std::unique_ptr<const sic::Filter> filter,
			   const sic::AbstractFilterNode *parentNode)
		: parentNode(parentNode), filter(std::move(filter)) {}

public:
	/**
	 * Create a root filter tree FilterNode.
	 *
	 * This node will implicitly be an AllAssetsFilter, passing any Asset.
	 */
	FilterNode()
		: parentNode(nullptr),
		  filter(std::make_unique<sic::AllAssetsFilter>()) {}

	~FilterNode() override {}

	sic::AbstractFilterNode &
	addChild(std::unique_ptr<const sic::Filter> childFilter) override;

	const sic::AbstractFilterNode *getParentNode() const override {
		return parentNode;
	}

	sic::Iterators<sic::AbstractFilterNode::ChildIterator>
	getChildIterators() override;
};

} // namespace sic

#endif // SIC_MODEL_FILTERNODE_H_
