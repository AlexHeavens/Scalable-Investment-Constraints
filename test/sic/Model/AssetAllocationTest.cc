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

	const sic::MockAssetAllocationNode aaNodeA;
	const sic::MockAssetAllocationNode aaNodeB;
	const sic::MockAssetAllocationNode aaNodeC;
	const sic::MockAssetAllocationNode aaNodeD;

	const sic::MockAssetAllocationNode aaNodeE;
	const sic::MockAssetAllocationNode aaNodeF;
	const sic::MockAssetAllocationNode aaNodeG;
	const sic::MockAssetAllocationNode aaNodeH;

	auto filterNodes = std::make_unique<sic::AssetAllocation::FilterNodeMap>();

	filterNodes->insert({&filterNodeA, &aaNodeA});
	filterNodes->insert({&filterNodeB, &aaNodeB});
	filterNodes->insert({&filterNodeC, &aaNodeC});
	filterNodes->insert({&filterNodeD, &aaNodeD});
	filterNodes->insert({&filterNodeE, &aaNodeE});
	filterNodes->insert({&filterNodeF, &aaNodeF});
	filterNodes->insert({&filterNodeG, &aaNodeG});
	filterNodes->insert({&filterNodeH, &aaNodeH});

	sic::AssetAllocation validAA(filterTree, std::move(filterNodes));

	ASSERT_EQ(&validAA.getFilterTree(), &filterTree);
}

} // namespace
