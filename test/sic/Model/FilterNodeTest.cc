#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "sic/Model/FilterNode.hh"
#include "sic/Model/MockFilter.hh"
#include "sic/Portfolio/MockAsset.hh"

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

TEST_F(FilterNodeTest, FilterToChild) {

	sic::FilterNode parentNode;

	std::vector<sic::AbstractFilterNode *> expChildNodes;
	expChildNodes.reserve(3);
	expChildNodes[0] =
		&parentNode.addChild(std::make_unique<sic::MockFilter>());
	expChildNodes[1] =
		&parentNode.addChild(std::make_unique<sic::MockFilter>());
	expChildNodes[2] =
		&parentNode.addChild(std::make_unique<sic::MockFilter>());

	sic::MockAsset testAsset;

	EXPECT_CALL(
		dynamic_cast<const sic::MockFilter &>(expChildNodes[0]->getFilter()),
		evaluate(testing::Ref(testAsset)))
		.Times(1)
		.WillOnce(testing::Return(false));
	EXPECT_CALL(
		dynamic_cast<const sic::MockFilter &>(expChildNodes[1]->getFilter()),
		evaluate(testing::Ref(testAsset)))
		.Times(1)
		.WillOnce(testing::Return(true));

	const sic::AbstractFilterNode *returnedNode =
		parentNode.filterToChild(testAsset);
	ASSERT_EQ(returnedNode, expChildNodes[1]);
}

} // namespace
