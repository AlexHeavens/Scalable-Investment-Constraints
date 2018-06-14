#include <gtest/gtest.h>

#include "sic/Model/AssetAllocation.hh"
#include "sic/Model/MockAssetAllocationNode.hh"
#include "sic/Model/MockFilterNode.hh"
#include "sic/Model/MockFilterTree.hh"

namespace {

class AssetAllocationTest : public testing::Test {};

TEST_F(AssetAllocationTest, CreateValid) {

	sic::MockFilterTree filterTree;

	const sic::MockFilterNode filterNodeA;
	const sic::MockFilterNode filterNodeB;
	const sic::MockFilterNode filterNodeC;
	const sic::MockFilterNode filterNodeD;

	const sic::MockFilterNode filterNodeE;
	const sic::MockFilterNode filterNodeF;
	const sic::MockFilterNode filterNodeG;
	const sic::MockFilterNode filterNodeH;

	auto aaNodeA = std::make_unique<sic::MockAssetAllocationNode>();
	auto aaNodeB = std::make_unique<sic::MockAssetAllocationNode>();
	auto aaNodeC = std::make_unique<sic::MockAssetAllocationNode>();
	auto aaNodeD = std::make_unique<sic::MockAssetAllocationNode>();

	auto aaNodeE = std::make_unique<sic::MockAssetAllocationNode>();
	auto aaNodeF = std::make_unique<sic::MockAssetAllocationNode>();
	auto aaNodeG = std::make_unique<sic::MockAssetAllocationNode>();
	auto aaNodeH = std::make_unique<sic::MockAssetAllocationNode>();

	auto filterNodes = std::make_unique<sic::AssetAllocation::FilterNodeMap>();

	filterNodes->insert(std::make_pair(&filterNodeA, std::move(aaNodeA)));
	filterNodes->insert(std::make_pair(&filterNodeB, std::move(aaNodeB)));
	filterNodes->insert(std::make_pair(&filterNodeC, std::move(aaNodeC)));
	filterNodes->insert(std::make_pair(&filterNodeD, std::move(aaNodeD)));
	filterNodes->insert(std::make_pair(&filterNodeE, std::move(aaNodeE)));
	filterNodes->insert(std::make_pair(&filterNodeF, std::move(aaNodeF)));
	filterNodes->insert(std::make_pair(&filterNodeG, std::move(aaNodeG)));
	filterNodes->insert(std::make_pair(&filterNodeH, std::move(aaNodeH)));

	sic::AssetAllocation validAA(filterTree, std::move(filterNodes));

	ASSERT_EQ(&validAA.getFilterTree(), &filterTree);
}

} // namespace
