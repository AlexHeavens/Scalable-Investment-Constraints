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

} // namespace
