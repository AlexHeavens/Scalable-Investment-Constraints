#include "sic/Model/RegularFilterTreeFactory.hh"

#include <stdexcept>

#include "sic/Model/Filter/AllAssetsFilter.hh"
#include "sic/Model/Filter/AssetClassFilter.hh"

namespace sic {

void RegularFilterTreeFactory::generateNode(sic::AbstractFilterNode &node,
											unsigned currentDepth) {

	if (currentDepth >= depth) {
		throw std::invalid_argument(
			"Attempted to generate node past tree depth.");
	}

	// Create node's children.
	auto nextClass = nextClassJump;
	for (unsigned childIndex = 0; childIndex < nodeDegree - 1; childIndex++) {

		const auto filterClass = nextClass++;
		auto filter = std::make_unique<sic::AssetClassFilter>(filterClass);
		node.addChild(std::move(filter));
	}

	// Always add an "All Assets" node at each level.
	auto filter = std::make_unique<sic::AllAssetsFilter>();
	node.addChild(std::move(filter));

	nextClassJump += nodeClassJump;
	const auto childDepth = currentDepth + 1;

	if (childDepth == depth) {
		return;
	}

	// Recurse to children, ignoring the last nodes that are AllAssets
	// catch-alls.
	for (unsigned childIndex = 0; childIndex < nodeDegree - 1; childIndex++) {
		auto &childNode = node.getChild(childIndex);
		generateNode(childNode, childDepth);
	}
}

std::unique_ptr<sic::FilterTree>
RegularFilterTreeFactory::create(sic::External::ID externalID) {

	auto returnTree = std::make_unique<sic::FilterTree>(externalID);

	create(*returnTree);

	return returnTree;
}

void RegularFilterTreeFactory::create(sic::FilterTree &filterTree) {

	constexpr unsigned currentDepth = 1;

	if (depth > currentDepth) {
		generateNode(filterTree.getRootNode(), currentDepth);
	}
}

std::unique_ptr<sic::AbstractAsset::ClassSet>
RegularFilterTreeFactory::getPathClasses(
	const std::vector<unsigned> &path) const {

	unsigned classGroup = 0;
	auto classes = std::make_unique<sic::AbstractAsset::ClassSet>();

	for (unsigned level = 1; level < depth; level++) {

		const auto classGroupID = classGroup * nodeClassJump;
		const auto classID = classGroupID + path.at(level - 1);

		classes->insert(classID);

		classGroup *= nodeDegree - 1;
		classGroup += path.at(level - 1) + 1;
	}

	return classes;
}

} // namespace sic
