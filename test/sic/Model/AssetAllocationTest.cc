#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "sic/Model/AssetAllocation.hh"

namespace {

class AssetAllocationTest : public testing::Test {

public:
	class MockFilterTree : public sic::AbstractFilterTree {
	public:
		MockFilterTree() : sic::AbstractFilterTree(1) {}
	};
	class MockFilterNode : public sic::AbstractFilterNode {
	public:
		sic::AbstractFilterNode &
		addChild(std::unique_ptr<const sic::Filter> childFilter) override {
			return addChild(childFilter.get());
		}

		MOCK_METHOD1(addChild, sic::AbstractFilterNode &(const sic::Filter *));
		MOCK_CONST_METHOD0(getParentNode, const sic::AbstractFilterNode *());
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

	const MockFilterNode filterNodeA;
	const MockFilterNode filterNodeB;
	const MockFilterNode filterNodeC;
	const MockFilterNode filterNodeD;

	const MockFilterNode filterNodeE;
	const MockFilterNode filterNodeF;
	const MockFilterNode filterNodeG;
	const MockFilterNode filterNodeH;

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
