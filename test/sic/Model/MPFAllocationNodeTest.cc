#include <gtest/gtest.h>

#include "sic/Model/MPFAllocationNode.hh"

namespace {

class MPFAllocationNodeTest : public testing::Test {

public:
	class MockModelPortfolio : public sic::AbstractModelPortfolio {};
};

TEST_F(MPFAllocationNodeTest, CreateValid) {

	const sic::WeightRange expRange(0.1, 0.1, 0.1);
	const MockModelPortfolio expMPF;

	sic::MPFAllocationNode validNode(expMPF, expRange);

	ASSERT_EQ(validNode.getWeightRange(), expRange);
	ASSERT_EQ(&validNode.getModelPortfolio(), &expMPF);
}

} // namespace
