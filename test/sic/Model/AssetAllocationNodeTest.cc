#include <gtest/gtest.h>

#include "sic/Model/AssetAllocationNode.hh"

namespace {

class AssetAllocationNodeTest : public testing::Test {

public:
};
TEST_F(AssetAllocationNodeTest, CreateValid) {

	const sic::WeightRange expRange(0.1, 0.2, 0.3);

	sic::AssetAllocationNode validNode(expRange);
	ASSERT_EQ(validNode.getWeightRange(), expRange);
}

} // namespace
