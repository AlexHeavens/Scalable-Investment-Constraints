#include "sic/Model/FilterTree.hh"

void sic::FilterTree::getAssetPath(const sic::AbstractAsset &asset,
								   sic::FilterTree::Path &path) {

	sic::AbstractFilterNode *currentNode = &(this->getRootNode());

	while (currentNode != nullptr) {

		path.push_back(currentNode);
		currentNode = currentNode->filterToChild(asset);
	}
}

sic::AbstractFilterNode &
sic::FilterTree::getLeafNode(const sic::AbstractAsset &asset) {

	sic::AbstractFilterNode *currentNode = &(this->getRootNode());
	sic::AbstractFilterNode *returnNode = nullptr;

	while (currentNode != nullptr) {

		returnNode = currentNode;
		currentNode = currentNode->filterToChild(asset);
	}

	return *returnNode;
}
