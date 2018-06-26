#include "sic/Model/FilterTree.hh"

#include "sic/Portfolio/ValueTree.hh"

namespace sic {

void FilterTree::getAssetPath(const sic::AbstractAsset &asset,
							  sic::FilterTree::Path &path) const {

	const sic::AbstractFilterNode *currentNode = &(this->getRootNode());

	while (currentNode != nullptr) {

		path.push_back(currentNode);
		currentNode = currentNode->filterToChild(asset);
	}
}

const sic::AbstractFilterNode &
FilterTree::getLeafNode(const sic::AbstractAsset &asset) const {

	const sic::AbstractFilterNode *currentNode = &(this->getRootNode());
	const sic::AbstractFilterNode *returnNode = nullptr;

	while (currentNode != nullptr) {

		returnNode = currentNode;
		currentNode = currentNode->filterToChild(asset);
	}

	return *returnNode;
}

std::unique_ptr<sic::AbstractValueTree>
FilterTree::evaluate(const sic::AbstractPortfolio &portfolio) const {

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

} // namespace sic
