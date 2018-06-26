#include <gtest/gtest.h>

#include "sic/Model/MPFAllocationNode.hh"
#include "sic/Model/MockModelPortfolio.hh"

namespace {

class MPFAllocationNodeTest : public testing::Test {};

TEST_F(MPFAllocationNodeTest, CreateValid) {

	const sic::WeightRange expRange(0.1, 0.1, 0.1);
	const sic::MockModelPortfolio expMPF;

	sic::MPFAllocationNode validNode(expMPF, expRange);

	ASSERT_EQ(validNode.getWeightRange(), expRange);
	ASSERT_EQ(&validNode.getModelPortfolio(), &expMPF);
}

} // namespace
