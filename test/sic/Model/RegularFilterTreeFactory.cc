#include "sic/Model/RegularFilterTreeFactory.hh"

#include <stdexcept>

#include "sic/Model/Filter/AllAssetsFilter.hh"
#include "sic/Model/Filter/AssetClassFilter.hh"

void sic::RegularFilterTreeFactory::generateNode(sic::AbstractFilterNode &node,
												 unsigned currentDepth) {

	if (currentDepth >= depth) {
		throw std::invalid_argument(
			"Attempted to generate node past tree depth.");
	}

	// Create node's children.
	for (unsigned childIndex = 0; childIndex < nodeDegree - 1; childIndex++) {

		const auto filterClass = nextClass++;
		auto filter = std::make_unique<sic::AssetClassFilter>(filterClass);
		node.addChild(std::move(filter));
	}

	// Always add an "All Assets" node at each level.
	auto filter = std::make_unique<sic::AllAssetsFilter>();
	node.addChild(std::move(filter));

	nextClassJump += nodeClassJump;
	currentDepth++;

	if (currentDepth == depth) {
		return;
	}

	// Recurse to new depth if necessary.
	auto childIterators = node.getChildIterators();
	while (childIterators.current() != childIterators.end()) {
		auto &childNode = **((childIterators.current()));
		generateNode(childNode, currentDepth);

		childIterators.current()++;
	}
}

std::unique_ptr<sic::FilterTree>
sic::RegularFilterTreeFactory::create(sic::External::ID externalID) {

	auto returnTree = std::make_unique<sic::FilterTree>(externalID);

	create(*returnTree);

	return returnTree;
}

void sic::RegularFilterTreeFactory::create(sic::FilterTree &filterTree) {

	constexpr unsigned currentDepth = 1;

	if (depth > currentDepth) {
		generateNode(filterTree.getRootNode(), currentDepth);
	}
}
