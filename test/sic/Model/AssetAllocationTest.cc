#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "sic/Model/AssetAllocation.hh"
#include "sic/Model/MockFilterNode.hh"

namespace {

class AssetAllocationTest : public testing::Test {

public:
	class MockFilterTree : public sic::AbstractFilterTree {
	public:
		MockFilterTree() : sic::AbstractFilterTree(1) {}
	};

	class MockAAParentNode : public sic::AbstractAssetAllocationNode {
	public:
		MOCK_CONST_METHOD0(getWeightRange, const sic::WeightRange &());
	};
	class MockAALeafNode : public sic::AbstractAssetAllocationNode {
	public:
		MOCK_CONST_METHOD0(getWeightRange, const sic::WeightRange &());
	};
};

TEST_F(AssetAllocationTest, CreateValid) {

	MockFilterTree filterTree;

	const sic::MockFilterNode filterNodeA;
	const sic::MockFilterNode filterNodeB;
	const sic::MockFilterNode filterNodeC;
	const sic::MockFilterNode filterNodeD;

	const sic::MockFilterNode filterNodeE;
	const sic::MockFilterNode filterNodeF;
	const sic::MockFilterNode filterNodeG;
	const sic::MockFilterNode filterNodeH;

	const MockAAParentNode aaNodeA;
	const MockAAParentNode aaNodeB;
	const MockAAParentNode aaNodeC;
	const MockAAParentNode aaNodeD;

	const MockAALeafNode aaNodeE;
	const MockAALeafNode aaNodeF;
	const MockAALeafNode aaNodeG;
	const MockAALeafNode aaNodeH;

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
