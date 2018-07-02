#include "sic/Model/RegularAAFactory.hh"

#include "sic/Model/AssetAllocationNode.hh"
#include "sic/Model/Filter/AllAssetsFilter.hh"
#include "sic/Model/MPFAllocationNode.hh"
#include "sic/Model/ModelPortfolio.hh"

namespace sic {

void RegularAAFactory::genLeafNodeAssetMap(
	const sic::AbstractFilterTree &filterTree,
	NodeAssets *leafNodeAssets) const {

	for (auto &node : filterTree) {

		// We will add an MPF for
		if (node.isLeaf()) {
			const auto &nodeFilter = node.getFilter();
			if (typeid(nodeFilter) != typeid(sic::AllAssetsFilter)) {
				const sic::AbstractFilterNode *aNode = &node;
				leafNodeAssets->insert(std::make_pair(
					aNode, std::vector<const sic::AbstractAsset *>()));
			}
		}
	}

	const auto leafCount = leafNodeAssets->size();
	std::size_t filledLeafCount = 0;
	std::default_random_engine randomEngine;
	std::uniform_int_distribution<std::size_t> assetIndexDistribution(
		0, assetSource.size() - 1);
	std::unordered_set<const sic::AbstractAsset *> seenAssets;

	while (filledLeafCount < leafCount) {

		const auto assetIndex = assetIndexDistribution(randomEngine);
		const auto &asset = *assetSource.at(assetIndex);

		if (seenAssets.find(&asset) != seenAssets.end()) {
			continue; // Randomly picked Asset twice, skip.
		}
		seenAssets.insert(&asset);

		const auto &leafNode = filterTree.getLeafNode(asset);

		const auto &nodeFilter = leafNode.getFilter();
		if (typeid(nodeFilter) == typeid(sic::AllAssetsFilter)) {
			continue; // AllAssets typically acts as catch all, not model.
		}

		auto &leafAssets = leafNodeAssets->at(&leafNode);
		if (leafAssets.size() < mpfAssetCount) {
			leafAssets.push_back(&asset);

			if (leafAssets.size() == mpfAssetCount) {
				filledLeafCount++;
			}
		}
	}
}

void RegularAAFactory::genMPFs(
	const NodeAssets &leafNodeAssets, NodeMPFMap *nodeMPFMap,
	std::vector<std::unique_ptr<sic::AbstractModelPortfolio>> *newMPFs) {

	const sic::Weight assetsTargetWeight = 1.0 / mpfAssetCount;
	const auto assetsWeightTol = assetsTargetWeight * TARGET_WEIGHT_TOL;
	const auto assetsMinWeight = assetsTargetWeight - assetsWeightTol;
	const auto assetsMaxWeight = assetsTargetWeight + assetsWeightTol;
	const sic::WeightRange assetsWeightRange(
		assetsMinWeight, assetsTargetWeight, assetsMaxWeight);

	for (auto leafAssets : leafNodeAssets) {
		const sic::AbstractFilterNode *const leafNode = leafAssets.first;
		const auto &assets = leafAssets.second;

		auto assetWeights =
			std::make_unique<sic::AbstractAsset::AssetWeightMap>();
		for (auto asset : assets) {
			assetWeights->insert(std::make_pair(asset, assetsWeightRange));
		}

		sic::AbstractModelPortfolio *mpf =
			new sic::ModelPortfolio(std::move(assetWeights), nextMPFID++);
		std::unique_ptr<sic::AbstractModelPortfolio> mpfPtr(mpf);
		newMPFs->push_back(std::move(mpfPtr));
		const auto nodeMPFPair = std::make_pair(leafNode, mpf);
		nodeMPFMap->insert(nodeMPFPair);
	}
}

void RegularAAFactory::genAANodes(
	const sic::AbstractFilterTree &filterTree, const NodeMPFMap &nodeMPFMap,
	sic::AssetAllocation::FilterNodeMap *filterNodeMap) const {

	// Add root AA node.
	const auto &rootNode = filterTree.getRootNode();
	const sic::WeightRange rootNodeWeights(1.0, 1.0, 1.0);
	auto rootAANode =
		std::make_unique<sic::AssetAllocationNode>(rootNodeWeights);
	filterNodeMap->insert(std::make_pair(&rootNode, std::move(rootAANode)));

	// Recursively add child AA nodes.
	genChildAANodes(rootNode, 1.0, nodeMPFMap, filterNodeMap);
}

void RegularAAFactory::genChildAANodes(
	const sic::AbstractFilterNode &node, const sic::Weight parentWeight,
	const NodeMPFMap &nodeMPFMap,
	sic::AssetAllocation::FilterNodeMap *filterNodeMap) const {

	const auto childCount = node.getChildCount();

	if (childCount <= 1) {
		return;
	}

	const sic::Weight childWeight =
		parentWeight / static_cast<sic::Weight>(childCount - 1);
	const sic::WeightRange nodeRange(childWeight, childWeight, childWeight);

	auto childIts = node.getChildIterators();
	for (std::size_t i = 0; i < childCount - 1; i++) {
		const auto &child = node.getChild(i);

		sic::AbstractAssetAllocationNode *newNodePtr = nullptr;
		if (child.isLeaf()) {
			const auto &mpf = *nodeMPFMap.at(&child);
			newNodePtr = new sic::MPFAllocationNode(mpf, nodeRange);
		} else {
			newNodePtr = new sic::AssetAllocationNode(nodeRange);
		}

		std::unique_ptr<sic::AbstractAssetAllocationNode> newNode(newNodePtr);
		filterNodeMap->insert(std::make_pair(&child, std::move(newNode)));

		childIts.current()++;
	}

	// Last child is expected to be catch-all filter, so target weight 0.
	const auto &lastChild = **childIts.current();

	const sic::WeightRange allAssetsRange(0.0, 0.0, 0.0);
	std::unique_ptr<sic::AbstractAssetAllocationNode> lastNode =
		std::make_unique<sic::AssetAllocationNode>(allAssetsRange);

	filterNodeMap->insert(std::make_pair(&lastChild, std::move(lastNode)));

	// Skip recursing through last node.
	for (std::size_t childIndex = 0; childIndex < childCount - 1;
		 childIndex++) {
		const auto &child = node.getChild(childIndex);

		genChildAANodes(child, childWeight, nodeMPFMap, filterNodeMap);
	}
}

RegularAAFactory::RegularAAFactory(
	sic::Source<std::unique_ptr<sic::AbstractFilterTree>> &filterTreeSource,
	const std::vector<std::unique_ptr<sic::AbstractAsset>> &assetSource,
	const std::size_t mpfAssetCount, sic::External::ID initialAAID,
	sic::External::ID initialMPFID)
	: filterTreeSource(filterTreeSource),
	  filterTrees(filterTreeSource.getItems()), assetSource(assetSource),
	  mpfAssetCount(mpfAssetCount), nextAAID(initialAAID),
	  nextMPFID(initialMPFID) {}

sic::RegularAAFactory::Result RegularAAFactory::create() {

	// Take FilterTrees in a cyclic manner from the source.
	if (!filterTrees.remaining()) {
		filterTrees = filterTreeSource.getItems();
	}
	const auto &filterTree = **filterTrees.current();
	filterTrees.current()++;

	// Create a mapping of node to a bounded yet random list of Assets that fall
	// under that node.
	NodeAssets leafNodeAssets;
	genLeafNodeAssetMap(filterTree, &leafNodeAssets);

	// Generate MPFs with Assets matching those picked per leaf node.
	auto newMPFs = std::make_unique<
		std::vector<std::unique_ptr<sic::AbstractModelPortfolio>>>();
	NodeMPFMap nodeMPFMap;
	genMPFs(leafNodeAssets, &nodeMPFMap, newMPFs.get());

	// Add AA nodes, including those with the new MPFs.
	auto filterNodeMap =
		std::make_unique<sic::AssetAllocation::FilterNodeMap>();
	genAANodes(filterTree, nodeMPFMap, filterNodeMap.get());

	// Bundle and return resulting AA / MPFs.
	std::unique_ptr<sic::AbstractAssetAllocation> newAA(
		new sic::AssetAllocation(filterTree, std::move(filterNodeMap),
								 nextAAID));
	nextAAID++;

	return {std::move(newAA), std::move(newMPFs)};
}

} // namespace sic
