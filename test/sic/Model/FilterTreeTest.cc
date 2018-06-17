#include <gtest/gtest.h>

#include <memory>

#include "sic/External.hh"
#include "sic/Model/FilterTree.hh"
#include "sic/Model/MockFilter.hh"
#include "sic/Portfolio/MockAsset.hh"

namespace {

class FilterTreeTest : public testing::Test {

public:
	static const sic::External::ID expExternalID = 1;
	sic::FilterTree validTree;

	std::vector<const sic::Filter *> filters;
	std::vector<sic::AbstractFilterNode *> childNodes;

	const sic::MockAsset testAsset;

	FilterTreeTest() : validTree(expExternalID) {}

	void SetUp() override {

		constexpr int nodeCount = 5;
		filters.resize(nodeCount);
		for (int i = 0; i < nodeCount; i++) {
			filters.at(i) = new sic::MockFilter();
		}

		auto rootNode = &validTree.getRootNode();

		childNodes.resize(nodeCount);
		childNodes.at(0) = &rootNode->addChild(
			std::unique_ptr<const sic::Filter>(filters.at(0)));
		childNodes.at(1) = &rootNode->addChild(
			std::unique_ptr<const sic::Filter>(filters.at(1)));
		childNodes.at(2) = &rootNode->addChild(
			std::unique_ptr<const sic::Filter>(filters.at(2)));
		childNodes.at(3) = &childNodes.at(0)->addChild(
			std::unique_ptr<const sic::Filter>(filters.at(3)));
		childNodes.at(4) = &childNodes.at(0)->addChild(
			std::unique_ptr<const sic::Filter>(filters.at(4)));
	}
};

TEST_F(FilterTreeTest, CreateValid) {

	// Couple of basic calls to check sanity.
	auto &rootNode = validTree.getRootNode();

	ASSERT_EQ(&rootNode.getFilterTree(), &validTree);
}

TEST_F(FilterTreeTest, FilterAssetPath) {

	EXPECT_CALL(
		dynamic_cast<const sic::MockFilter &>(childNodes.at(0)->getFilter()),
		evaluate(testing::Ref(testAsset)))
		.Times(1)
		.WillOnce(testing::Return(true));
	EXPECT_CALL(
		dynamic_cast<const sic::MockFilter &>(childNodes.at(3)->getFilter()),
		evaluate(testing::Ref(testAsset)))
		.Times(1)
		.WillOnce(testing::Return(false));
	EXPECT_CALL(
		dynamic_cast<const sic::MockFilter &>(childNodes.at(4)->getFilter()),
		evaluate(testing::Ref(testAsset)))
		.Times(1)
		.WillOnce(testing::Return(true));

	sic::FilterTree::Path path;
	validTree.getAssetPath(testAsset, path);

	ASSERT_EQ(path.size(), 3);
	ASSERT_EQ(path.at(0), &validTree.getRootNode());
	ASSERT_EQ(path.at(1), childNodes.at(0));
	ASSERT_EQ(path.at(2), childNodes.at(4));
}

TEST_F(FilterTreeTest, GetLeafNode) {

	EXPECT_CALL(
		dynamic_cast<const sic::MockFilter &>(childNodes.at(0)->getFilter()),
		evaluate(testing::Ref(testAsset)))
		.Times(1)
		.WillOnce(testing::Return(true));
	EXPECT_CALL(
		dynamic_cast<const sic::MockFilter &>(childNodes.at(3)->getFilter()),
		evaluate(testing::Ref(testAsset)))
		.Times(1)
		.WillOnce(testing::Return(false));
	EXPECT_CALL(
		dynamic_cast<const sic::MockFilter &>(childNodes.at(4)->getFilter()),
		evaluate(testing::Ref(testAsset)))
		.Times(1)
		.WillOnce(testing::Return(true));

	auto &returnedLeafNode = validTree.getLeafNode(testAsset);
	ASSERT_EQ(&returnedLeafNode, childNodes.at(4));
}

} // namespace
