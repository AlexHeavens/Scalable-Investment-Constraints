#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "sic/Model/FilterNode.hh"
#include "sic/Model/MockFilter.hh"
#include "sic/Model/MockFilterTree.hh"
#include "sic/Portfolio/MockAsset.hh"

namespace {

class FilterNodeTest : public testing::Test {

public:
	sic::MockFilterTree filterTree;
	sic::FilterNode parentNode;
	std::vector<sic::AbstractFilterNode *> expChildNodes;

	FilterNodeTest() : parentNode(filterTree, 0) {}

	void SetUp() override {
		expChildNodes.resize(3);
		expChildNodes.at(0) =
			&parentNode.addChild(std::make_unique<sic::MockFilter>());
		expChildNodes.at(1) =
			&parentNode.addChild(std::make_unique<sic::MockFilter>());
		expChildNodes.at(2) =
			&parentNode.addChild(std::make_unique<sic::MockFilter>());
	}
};

TEST_F(FilterNodeTest, CreateValid) {

	sic::MockFilterTree filterTree;

	sic::FilterNode parentNode(filterTree, 0);
	ASSERT_EQ(&parentNode.getFilterTree(), &filterTree);
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

	for (const auto &childNode : expChildNodes) {
		ASSERT_EQ(&childNode->getFilterTree(), &filterTree);
	}
}

TEST_F(FilterNodeTest, FilterToChild) {

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

TEST_F(FilterNodeTest, getChild) {

	ASSERT_EQ(&parentNode.getChild(0), expChildNodes.at(0));
	ASSERT_EQ(&parentNode.getChild(1), expChildNodes.at(1));
	ASSERT_EQ(&parentNode.getChild(2), expChildNodes.at(2));
}

TEST_F(FilterNodeTest, isLeaf) {

	ASSERT_FALSE(parentNode.isLeaf());
	ASSERT_TRUE(expChildNodes.at(0)->isLeaf());
	ASSERT_TRUE(expChildNodes.at(1)->isLeaf());
	ASSERT_TRUE(expChildNodes.at(2)->isLeaf());
}

} // namespace
