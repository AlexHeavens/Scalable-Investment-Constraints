#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "sic/Model/FilterNode.hh"

namespace {

class FilterNodeTest : public testing::Test {

public:
	class MockFilter : public sic::Model::Filter {

	public:
		MOCK_CONST_METHOD1(evaluate, bool(const sic::AbstractAsset &));
	};
};
TEST_F(FilterNodeTest, CreateValid) {

	sic::Model::FilterNode parentNode;

	std::vector<sic::Model::AbstractFilterNode *> expChildNodes;
	expChildNodes.reserve(3);
	expChildNodes[0] = &parentNode.addChild(std::make_unique<MockFilter>());
	expChildNodes[1] = &parentNode.addChild(std::make_unique<MockFilter>());
	expChildNodes[2] =
		&expChildNodes[1]->addChild(std::make_unique<MockFilter>());

	const sic::Model::AbstractFilterNode *nullNode = nullptr;
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
