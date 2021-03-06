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
	const sic::AbstractFilterTree &filterTree;
	sic::AbstractFilterNode::ChildNodeVector childNodes;
	const std::unique_ptr<const sic::Filter> filter;
	const std::size_t parentIndex;

	/**
	 * Create a non-root filter tree FilterNode.
	 *
	 * This is private to ensure the consistency of the parentNode pointer.
	 *
	 * @param parentFilterNode the parent FilterNode in the filter tree.
	 */
	FilterNode(const sic::AbstractFilterTree &filterTree,
			   std::unique_ptr<const sic::Filter> filter,
			   const sic::AbstractFilterNode *parentNode,
			   std::size_t parentIndex)
		: parentNode(parentNode), filterTree(filterTree),
		  filter(std::move(filter)), parentIndex(parentIndex) {}

public:
	/**
	 * Create a root filter tree FilterNode.
	 *
	 * This node will implicitly be an AllAssetsFilter, passing any Asset.
	 */
	FilterNode(const sic::AbstractFilterTree &filterTree, size_t parentIndex)
		: parentNode(nullptr), filterTree(filterTree),
		  filter(std::make_unique<sic::AllAssetsFilter>()),
		  parentIndex(parentIndex) {}

	const sic::Filter &getFilter() const override { return *filter; }

	sic::AbstractFilterNode &
	addChild(std::unique_ptr<const sic::Filter> childFilter) override;

	const sic::AbstractFilterNode *getParentNode() const override {
		return parentNode;
	}

	const sic::AbstractFilterNode *
	filterToChild(const sic::AbstractAsset &asset) const override;

	sic::Iterators<std::unique_ptr<sic::AbstractFilterNode>>
	getChildIterators() override;

	sic::Iterators<std::unique_ptr<sic::AbstractFilterNode>>
	getChildIterators() const override;

	std::size_t getChildCount() const override { return childNodes.size(); };

	const sic::AbstractFilterTree &getFilterTree() const override {
		return filterTree;
	}

	bool isLeaf() const override { return childNodes.empty(); }

	virtual std::size_t getParentIndex() const override { return parentIndex; }

	sic::AbstractFilterNode &getChild(size_t index) const override {
		return *childNodes.at(index);
	}
};

} // namespace sic

#endif // SIC_MODEL_FILTERNODE_H_
