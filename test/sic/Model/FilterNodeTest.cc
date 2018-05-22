#include <gtest/gtest.h>

#include "sic/Model/FilterNode.hh"

namespace {

class FilterNodeTest : public testing::Test {};
TEST_F(FilterNodeTest, CreateValidFilterNode) {

	sic::Model::FilterNode parentNode;
	sic::Model::AbstractFilterNode &childNode1 = parentNode.addChild();
	sic::Model::AbstractFilterNode &childNode2 = parentNode.addChild();
	sic::Model::AbstractFilterNode &childNode3 = childNode1.addChild();

	const sic::Model::AbstractFilterNode *nullNode = nullptr;
	ASSERT_EQ(nullNode, parentNode.getParentNode());
	ASSERT_EQ(&parentNode, childNode1.getParentNode());
	ASSERT_EQ(&parentNode, childNode2.getParentNode());
	ASSERT_EQ(&childNode1, childNode3.getParentNode());
}

} // namespace
