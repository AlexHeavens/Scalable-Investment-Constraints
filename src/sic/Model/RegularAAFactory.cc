#include "sic/Model/RegularAAFactory.hh"

#include "sic/Model/AssetAllocationNode.hh"

namespace sic {

void sic::RegularAAFactory::addAANodes(
	const sic::AbstractFilterNode &node, sic::Weight parentWeight,
	sic::AssetAllocation::FilterNodeMap *filterNodeMap) {

	const int childCount = node.getChildCount();

	// The FilterTree is expected to be regular with one child node acting
	// as a catch-all.  If the current not has only the catch-all, or no
	// children, we have reached the leaves of the tree.
	if (childCount <= 1) {
		return;
	}

	const sic::Weight childWeight =
		parentWeight / static_cast<sic::Weight>(childCount - 1);

	auto childIts = node.getChildIterators();
	for (int i = 0; i < childCount - 1; i++) {
		const auto &child = **childIts.current();

		const sic::WeightRange nodeRange(childWeight, childWeight, childWeight);
		std::unique_ptr<sic::AbstractAssetAllocationNode> newNode =
			std::make_unique<sic::AssetAllocationNode>(nodeRange);

		filterNodeMap->insert(std::make_pair(&child, std::move(newNode)));

		childIts.current()++;
	}

	// Last child is expected to be catch-all filter, so target weight 0.
	const auto &lastChild = **childIts.current();

	const sic::WeightRange nodeRange(0.0, 0.0, 0.0);
	std::unique_ptr<sic::AbstractAssetAllocationNode> lastNode =
		std::make_unique<sic::AssetAllocationNode>(nodeRange);

	filterNodeMap->insert(std::make_pair(&lastChild, std::move(lastNode)));

	childIts = node.getChildIterators();
	for (int i = 0; i < childCount - 1; i++) {
		const auto &child = **childIts.current();

		addAANodes(child, childWeight, filterNodeMap);

		childIts.current()++;
	}
}

std::unique_ptr<sic::AbstractAssetAllocation> sic::RegularAAFactory::create() {

	// Take FilterTrees in a cyclic manner from the source.
	if (!filterTrees.remaining()) {
		filterTrees = filterTreeSource.getItems();
	}

	const auto &filterTree = **filterTrees.current();
	filterTrees.current()++;

	auto filterNodeMap =
		std::make_unique<sic::AssetAllocation::FilterNodeMap>();

	const auto &rootNode = filterTree.getRootNode();
	const sic::WeightRange rootNodeWeights(1.0, 1.0, 1.0);
	auto rootAANode =
		std::make_unique<sic::AssetAllocationNode>(rootNodeWeights);
	filterNodeMap->insert(std::make_pair(&rootNode, std::move(rootAANode)));

	// Recursively add child AA nodes.
	addAANodes(rootNode, 1.0, filterNodeMap.get());

	sic::AssetAllocation *newAA = new sic::AssetAllocation(
		filterTree, std::move(filterNodeMap), nextAAID);

	nextAAID++;

	std::unique_ptr<sic::AbstractAssetAllocation> newAAPtr(newAA);

	return newAAPtr;
}

} // namespace sic
