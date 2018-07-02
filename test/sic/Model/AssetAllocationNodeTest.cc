#include <gtest/gtest.h>

#include "sic/Model/AssetAllocationNode.hh"

namespace {

class AssetAllocationNodeTest : public testing::Test {};

TEST_F(AssetAllocationNodeTest, CreateValid) {

	const sic::WeightRange expRange(0.1, 0.2, 0.3);
	const sic::External::ID expID = 123;

	sic::AssetAllocationNode validNode(expRange, expID);

	ASSERT_EQ(validNode.getWeightRange(), expRange);
	ASSERT_EQ(validNode.getExternalID(), expID);
}

} // namespace
