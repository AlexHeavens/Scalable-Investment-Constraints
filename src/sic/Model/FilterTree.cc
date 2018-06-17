#include "sic/Model/FilterTree.hh"

#include "sic/Portfolio/ValueTree.hh"

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
