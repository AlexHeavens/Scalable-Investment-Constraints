#include <gtest/gtest.h>

#include "sic/Model/AssetAllocation.hh"
#include "sic/Model/MockAssetAllocationNode.hh"
#include "sic/Model/MockFilterNode.hh"
#include "sic/Model/MockFilterTree.hh"

namespace {

class AssetAllocationTest : public testing::Test {};

TEST_F(AssetAllocationTest, CreateValid) {

	sic::MockFilterTree filterTree;

	constexpr int filterNodeCount = 8;

	std::vector<std::unique_ptr<sic::MockFilterNode>> filterNodes;
	filterNodes.reserve(filterNodeCount);

	for (int i = 0; i < filterNodeCount; i++) {
		filterNodes.emplace_back(new sic::MockFilterNode);
		EXPECT_CALL(*filterNodes.at(i), getFilterTree())
			.WillOnce(testing::ReturnRef(filterTree));
	}

	std::vector<std::unique_ptr<sic::MockAssetAllocationNode>> aaNodes;
	aaNodes.reserve(filterNodeCount);

	for (int i = 0; i < filterNodeCount; i++) {
		aaNodes.emplace_back(new sic::MockAssetAllocationNode);
	}

	auto filterNodeMap =
		std::make_unique<sic::AssetAllocation::FilterNodeMap>();

	for (int i = 0; i < filterNodeCount; i++) {
		filterNodeMap->insert(
			std::make_pair(filterNodes.at(i).get(), std::move(aaNodes.at(i))));
	}

	sic::AssetAllocation validAA(filterTree, std::move(filterNodeMap));

	ASSERT_EQ(&validAA.getFilterTree(), &filterTree);
}

TEST_F(AssetAllocationTest, MapUnknownFilterTreeNode) {

	sic::MockFilterNode knownNode1, unknownNode, knownNode2;
	const sic::MockFilterTree filterTree;
	const sic::MockFilterTree unknownFilterTree;

	auto aaNode1 = std::make_unique<sic::MockAssetAllocationNode>();
	auto aaNode2 = std::make_unique<sic::MockAssetAllocationNode>();
	auto aaNode3 = std::make_unique<sic::MockAssetAllocationNode>();

	auto filterNodeMap =
		std::make_unique<sic::AssetAllocation::FilterNodeMap>();
	filterNodeMap->insert(std::make_pair(&knownNode1, std::move(aaNode1)));
	filterNodeMap->insert(std::make_pair(&unknownNode, std::move(aaNode2)));
	filterNodeMap->insert(std::make_pair(&knownNode2, std::move(aaNode3)));

	// Difficult to predict path through node map, so may or may not query other
	// nodes for their filter tree.  Results in some warnings in testing.
	ON_CALL(knownNode1, getFilterTree())
		.WillByDefault(testing::ReturnRef(filterTree));
	ON_CALL(knownNode2, getFilterTree())
		.WillByDefault(testing::ReturnRef(filterTree));

	EXPECT_CALL(unknownNode, getFilterTree())
		.WillOnce(testing::ReturnRef(unknownFilterTree));

	const std::string expExceptionString =
		"filterNodeMap contains FilterNode from an unknown tree.";
	try {
		sic::AssetAllocation aa(filterTree, std::move(filterNodeMap));
#pragma unused(aa)

		FAIL() << "Able to pass filter tree node from unknown tree.";
	} catch (const std::invalid_argument &e) {
		ASSERT_EQ(expExceptionString, e.what());
	} catch (const std::exception &e) {
		FAIL() << "Unexpected  error: " << e.what() << "\n";
	}
}

} // namespace
