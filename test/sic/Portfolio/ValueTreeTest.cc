#include <gtest/gtest.h>

#include <vector>

#include "sic/Model/MockFilterNode.hh"
#include "sic/Portfolio/ValueTree.hh"

namespace {

class ValueTreeTest : public testing::Test {};

TEST_F(ValueTreeTest, CreateValid) {

	const int filterNodeCount = 10;
	std::vector<std::unique_ptr<sic::MockFilterNode>> filterNodes;

	filterNodes.reserve(filterNodeCount);
	for (int i = 0; i < filterNodeCount; i++) {
		filterNodes.emplace_back(new sic::MockFilterNode());
	}

	std::vector<sic::Weight> nodeWeights;

	nodeWeights.reserve(filterNodeCount);
	for (int i = 0; i < filterNodeCount; i++) {
		nodeWeights.push_back(0.1);
	}

	auto nodeWeightMap = std::make_unique<sic::ValueTree::NodeWeightMap>();

	for (int i = 0; i < filterNodeCount; i++) {

		const auto filterNodePtr = filterNodes.at(i).get();
		const auto nodeWeight = nodeWeights.at(i);

		nodeWeightMap->insert(std::make_pair(filterNodePtr, nodeWeight));
	}

	sic::ValueTree valueTree(std::move(nodeWeightMap));

	for (int i = 0; i < filterNodeCount; i++) {
		auto nodeWeight = valueTree.getNodeWeight(*filterNodes.at(i));
		ASSERT_EQ(nodeWeight, nodeWeights.at(i));
	}
}

TEST_F(ValueTreeTest, Equals) {

	const int filterNodeCount = 5;
	std::vector<std::unique_ptr<sic::MockFilterNode>> filterNodes;

	filterNodes.reserve(filterNodeCount);
	for (int i = 0; i < filterNodeCount; i++) {
		filterNodes.emplace_back(new sic::MockFilterNode());
	}

	auto nodeWeightMapA = std::make_unique<sic::ValueTree::NodeWeightMap>();
	auto nodeWeightMapB = std::make_unique<sic::ValueTree::NodeWeightMap>();
	auto nodeWeightMapC = std::make_unique<sic::ValueTree::NodeWeightMap>();
	for (int i = 0; i < filterNodeCount; i++) {
		nodeWeightMapA->insert(std::make_pair(filterNodes.at(i).get(), 0.2));
		nodeWeightMapB->insert(std::make_pair(filterNodes.at(i).get(), 0.2));

		const sic::Weight filterNodeCWeight = (i <= 1) ? 0.05 : 0.3;
		nodeWeightMapC->insert(
			std::make_pair(filterNodes.at(i).get(), filterNodeCWeight));
	}

	sic::ValueTree treeA(std::move(nodeWeightMapA));
	sic::ValueTree treeB(std::move(nodeWeightMapB));
	sic::ValueTree treeC(std::move(nodeWeightMapC));

	ASSERT_TRUE(treeA == treeB) << "Identical ValueTrees don't match.";
	ASSERT_FALSE(treeA == treeC) << "Non-identical ValueTrees match.";

	auto nodeWeightMapD = std::make_unique<sic::ValueTree::NodeWeightMap>();
	auto nodeWeightMapE = std::make_unique<sic::ValueTree::NodeWeightMap>();
	sic::ValueTree treeD(std::move(nodeWeightMapD));
	sic::ValueTree treeE(std::move(nodeWeightMapE));

	ASSERT_TRUE(treeD == treeE) << "Empty ValueTrees don't match.";
}

} // namespace
