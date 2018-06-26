#include "sic/Model/FilterTree.hh"

#include "sic/Portfolio/ValueTree.hh"

sic::AbstractFilterNode *sic::FilterTree::node_iterator::getNextNode() const {

	sic::AbstractFilterNode *nextNode = nullptr;

	// Depth-first search.  Keep taking descendent nodes until we hit a leaf.
	if (!currentNode->isLeaf()) {

		nextNode = &currentNode->getChild(0);

	} else {

		// Try to take the next child of the current node's parent.
		const auto *parentNode = currentNode->getParentNode();
		const auto *prevNode = currentNode;
		const auto prevNodeIndex = prevNode->getParentIndex();
		auto nextNodeIndex = prevNodeIndex + 1;

		// When we reach the end a parent node's children, recurse up the tree
		// until we find a parent with remaining children to visit.
		while (parentNode != nullptr and
			   nextNodeIndex == parentNode->getChildCount()) {
			prevNode = parentNode;
			parentNode = parentNode->getParentNode();

			if (parentNode != nullptr) {

				const auto prevNodeIndex = prevNode->getParentIndex();
				nextNodeIndex = prevNodeIndex + 1;
			}
		}

		// nullptr parent indicates root node; the iteration is complete.
		if (parentNode != nullptr) {
			nextNode = &parentNode->getChild(nextNodeIndex);
		}
	}

	return nextNode;
}

void sic::FilterTree::getAssetPath(const sic::AbstractAsset &asset,
								   sic::FilterTree::Path &path) const {

	const sic::AbstractFilterNode *currentNode = &(this->getRootNode());

	while (currentNode != nullptr) {

		path.push_back(currentNode);
		currentNode = currentNode->filterToChild(asset);
	}
}

const sic::AbstractFilterNode &
sic::FilterTree::getLeafNode(const sic::AbstractAsset &asset) const {

	const sic::AbstractFilterNode *currentNode = &(this->getRootNode());
	const sic::AbstractFilterNode *returnNode = nullptr;

	while (currentNode != nullptr) {

		returnNode = currentNode;
		currentNode = currentNode->filterToChild(asset);
	}

	return *returnNode;
}

std::unique_ptr<sic::AbstractValueTree>
sic::FilterTree::evaluate(const sic::AbstractPortfolio &portfolio) const {

	auto nodeWeightMap = std::make_unique<sic::ValueTree::NodeWeightMap>();

	const auto totalPortfolioValue = portfolio.getTotalReferenceValue();

	auto positions = portfolio.getPositionIterators();
	while (positions.remaining()) {
		const auto &position = *positions.current();

		const auto &asset = position.getAsset();
		const auto &assetLeafNode = getLeafNode(asset);
		const sic::Weight positionWeight =
			position.getReferenceValue() / totalPortfolioValue;

		// Add position weight to all nodes in its tree path.
		const sic::AbstractFilterNode *pathNode = &assetLeafNode;
		while (pathNode != nullptr) {

			auto mapNodeLookup = nodeWeightMap->find(pathNode);
			if (mapNodeLookup != nodeWeightMap->end()) {
				const auto prevNodeWeight = mapNodeLookup->second;
				(*nodeWeightMap)[pathNode] = prevNodeWeight + positionWeight;
			} else {
				nodeWeightMap->insert(std::pair(pathNode, positionWeight));
			}

			pathNode = pathNode->getParentNode();
		}

		positions.current()++;
	}

	auto valueTreePtr = new sic::ValueTree(std::move(nodeWeightMap));
	std::unique_ptr<sic::AbstractValueTree> valueTree(valueTreePtr);

	return valueTree;
}
