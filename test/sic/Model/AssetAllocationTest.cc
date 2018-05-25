#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "sic/Model/AssetAllocation.hh"

namespace {

class AssetAllocationTest : public testing::Test {

public:
	class MockFilterTree : public sic::Model::AbstractFilterTree {};
	class MockFilterNode : public sic::Model::AbstractFilterNode {
	public:
		sic::Model::AbstractFilterNode &addChild(
			std::unique_ptr<const sic::Model::Filter> childFilter) override {
			return addChild(childFilter.get());
		}

		MOCK_METHOD1(addChild, sic::Model::AbstractFilterNode &(
								   const sic::Model::Filter *));
		MOCK_CONST_METHOD0(getParentNode,
						   const sic::Model::AbstractFilterNode *());
	};

	class MockAAParentNode : public sic::Model::AbstractAssetAllocationNode {
	public:
		MOCK_CONST_METHOD0(getWeightRange, const sic::WeightRange &());
	};
	class MockAALeafNode : public sic::Model::AbstractAssetAllocationNode {
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

	auto filterNodes =
		std::make_unique<sic::Model::AssetAllocation::FilterNodeMap>();

	filterNodes->insert({&filterNodeA, &aaNodeA});
	filterNodes->insert({&filterNodeB, &aaNodeB});
	filterNodes->insert({&filterNodeC, &aaNodeC});
	filterNodes->insert({&filterNodeD, &aaNodeD});
	filterNodes->insert({&filterNodeE, &aaNodeE});
	filterNodes->insert({&filterNodeF, &aaNodeF});
	filterNodes->insert({&filterNodeG, &aaNodeG});
	filterNodes->insert({&filterNodeH, &aaNodeH});

	sic::Model::AssetAllocation validAA(filterTree, std::move(filterNodes));

	ASSERT_EQ(&validAA.getFilterTree(), &filterTree);
}

} // namespace
