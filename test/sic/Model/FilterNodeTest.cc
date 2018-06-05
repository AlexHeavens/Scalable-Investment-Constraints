#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "sic/Model/FilterNode.hh"
#include "sic/Model/MockFilter.hh"

namespace {

class FilterNodeTest : public testing::Test {};

TEST_F(FilterNodeTest, CreateValid) {

	sic::FilterNode parentNode;
	ASSERT_EQ(parentNode.getChildCount(), 0);

	std::vector<sic::AbstractFilterNode *> expChildNodes;
	expChildNodes.reserve(3);
	expChildNodes[0] =
		&parentNode.addChild(std::make_unique<sic::MockFilter>());
	ASSERT_EQ(parentNode.getChildCount(), 1);
	expChildNodes[1] =
		&parentNode.addChild(std::make_unique<sic::MockFilter>());
	ASSERT_EQ(parentNode.getChildCount(), 2);
	ASSERT_EQ(expChildNodes[1]->getChildCount(), 0);
	expChildNodes[2] =
		&expChildNodes[1]->addChild(std::make_unique<sic::MockFilter>());
	ASSERT_EQ(expChildNodes[1]->getChildCount(), 1);

	const sic::AbstractFilterNode *nullNode = nullptr;
	ASSERT_EQ(nullNode, parentNode.getParentNode());
	ASSERT_EQ(&parentNode, expChildNodes[0]->getParentNode());
	ASSERT_EQ(&parentNode, expChildNodes[1]->getParentNode());
	ASSERT_EQ(expChildNodes[1], expChildNodes[2]->getParentNode());

	auto childIterators = parentNode.getChildIterators();
	ASSERT_EQ(childIterators.current()->get(), expChildNodes[0]);
	childIterators.current()++;

	ASSERT_EQ(childIterators.current()->get(), expChildNodes[1]);
	childIterators.current()++;

	ASSERT_EQ(childIterators.current(), childIterators.end());
}

} // namespace
