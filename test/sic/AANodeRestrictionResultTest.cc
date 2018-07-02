#include <gtest/gtest.h>

#include "sic/AANodeRestrictionResult.hh"
#include "sic/Model/MockAssetAllocationNode.hh"

namespace {

class AANodeRestrictionResultTest : public testing::Test {};

TEST_F(AANodeRestrictionResultTest, CreatePass) {

	const sic::External::ID aaNodeID = 123;
	sic::MockAssetAllocationNode aaNode(aaNodeID);
	const sic::WeightRange aaNodeWeightRange(0.1, 0.2, 0.3);
	const sic::Weight nodeWeight = 0.3;

	EXPECT_CALL(aaNode, getWeightRange())
		.Times(3)
		.WillRepeatedly(testing::ReturnRef(aaNodeWeightRange));

	sic::AANodeRestrictionResult result(aaNode, nodeWeight);

	ASSERT_EQ(result.getState(), sic::RestrictionResult::State::Pass);
	const std::string expSerialisation =
		"AssetAllocationNode,0,123,0.300000,0.100000,0.200000,0.300000\n";

	ASSERT_EQ(result.serialise(), expSerialisation);
}

TEST_F(AANodeRestrictionResultTest, CreateFail) {

	const sic::External::ID aaNodeID = 123;
	sic::MockAssetAllocationNode aaNode(aaNodeID);
	const sic::WeightRange aaNodeWeightRange(0.1, 0.2, 0.3);
	const sic::Weight nodeWeight = 0.300001;

	EXPECT_CALL(aaNode, getWeightRange())
		.Times(3)
		.WillRepeatedly(testing::ReturnRef(aaNodeWeightRange));

	sic::AANodeRestrictionResult result(aaNode, nodeWeight);

	ASSERT_EQ(result.getState(), sic::RestrictionResult::State::Fail);
	const std::string expSerialisation =
		"AssetAllocationNode,1,123,0.300001,0.100000,0.200000,0.300000\n";

	ASSERT_EQ(result.serialise(), expSerialisation);
}

} // namespace
