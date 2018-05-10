#include <gtest/gtest.h>

#include "sic/Model/Node.hh"

namespace {

class NodeTest : public testing::Test {};
TEST_F(NodeTest, CreateValidNode) {

	sic::Model::Node parentNode;
	sic::Model::Node &childNode1 = parentNode.addChild();
	sic::Model::Node &childNode2 = parentNode.addChild();
	sic::Model::Node &childNode3 = childNode1.addChild();

	const sic::Model::Node *nullNode = nullptr;
	ASSERT_EQ(nullNode, parentNode.getParentNode());
	ASSERT_EQ(&parentNode, childNode1.getParentNode());
	ASSERT_EQ(&parentNode, childNode2.getParentNode());
	ASSERT_EQ(&childNode1, childNode3.getParentNode());
}

} // namespace
