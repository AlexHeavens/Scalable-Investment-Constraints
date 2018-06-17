#include <gtest/gtest.h>

#include <memory>

#include "sic/External.hh"
#include "sic/MockPortfolio.hh"
#include "sic/Model/FilterTree.hh"
#include "sic/Model/MockFilter.hh"
#include "sic/Portfolio.hh"
#include "sic/Portfolio/MockAsset.hh"
#include "sic/Portfolio/MockPosition.hh"
#include "sic/Portfolio/Position.hh"
#include "sic/Portfolio/ValueTree.hh"

namespace {

class FilterTreeTest : public testing::Test {

public:
	static const sic::External::ID expExternalID = 1;
	sic::FilterTree validTree;

	std::vector<const sic::Filter *> filters;
	std::vector<sic::AbstractFilterNode *> childNodes;
	sic::AbstractFilterNode *rootNode = nullptr;

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
	ASSERT_EQ(path.at(0), rootNode);
	ASSERT_EQ(path.at(1), childNodes.at(0));
	ASSERT_EQ(path.at(2), childNodes.at(4));
}

TEST_F(FilterTreeTest, GetLeafNode) {

	EXPECT_CALL(
		dynamic_cast<const sic::MockFilter &>(childNodes.at(0)->getFilter()),
		evaluate(testing::Ref(testAsset)))
		.WillOnce(testing::Return(true));
	EXPECT_CALL(
		dynamic_cast<const sic::MockFilter &>(childNodes.at(3)->getFilter()),
		evaluate(testing::Ref(testAsset)))
		.WillOnce(testing::Return(false));
	EXPECT_CALL(
		dynamic_cast<const sic::MockFilter &>(childNodes.at(4)->getFilter()),
		evaluate(testing::Ref(testAsset)))
		.WillOnce(testing::Return(true));

	auto &returnedLeafNode = validTree.getLeafNode(testAsset);
	ASSERT_EQ(&returnedLeafNode, childNodes.at(4));
}

TEST_F(FilterTreeTest, EvaluatePortfolio) {

	std::vector<sic::MockAsset> assets;

	constexpr int assetCount = 4;
	assets.reserve(assetCount);
	for (int i = 0; i < assetCount; i++) {
		assets.emplace_back(i);
	}

	auto positions = std::make_unique<std::vector<sic::Position>>();

	constexpr int positionCount = 5;
	positions->reserve(positionCount);

	positions->emplace_back(assets.at(0), 0.3, 0);
	positions->emplace_back(assets.at(1), 0.2, 1);
	positions->emplace_back(assets.at(2), 0.2, 2);
	positions->emplace_back(assets.at(3), 0.1, 3);
	positions->emplace_back(assets.at(0), 0.2, 4);

	sic::Portfolio portfolio(std::move(positions), 123);

	EXPECT_CALL(
		dynamic_cast<const sic::MockFilter &>(childNodes.at(0)->getFilter()),
		evaluate(testing::Ref(assets.at(0))))
		.Times(2)
		.WillRepeatedly(testing::Return(true));
	EXPECT_CALL(
		dynamic_cast<const sic::MockFilter &>(childNodes.at(3)->getFilter()),
		evaluate(testing::Ref(assets.at(0))))
		.Times(2)
		.WillRepeatedly(testing::Return(true));

	EXPECT_CALL(
		dynamic_cast<const sic::MockFilter &>(childNodes.at(0)->getFilter()),
		evaluate(testing::Ref(assets.at(1))))
		.WillOnce(testing::Return(true));
	EXPECT_CALL(
		dynamic_cast<const sic::MockFilter &>(childNodes.at(3)->getFilter()),
		evaluate(testing::Ref(assets.at(1))))
		.WillOnce(testing::Return(false));
	EXPECT_CALL(
		dynamic_cast<const sic::MockFilter &>(childNodes.at(4)->getFilter()),
		evaluate(testing::Ref(assets.at(1))))
		.WillOnce(testing::Return(true));

	EXPECT_CALL(
		dynamic_cast<const sic::MockFilter &>(childNodes.at(0)->getFilter()),
		evaluate(testing::Ref(assets.at(2))))
		.WillOnce(testing::Return(false));
	EXPECT_CALL(
		dynamic_cast<const sic::MockFilter &>(childNodes.at(1)->getFilter()),
		evaluate(testing::Ref(assets.at(2))))
		.WillOnce(testing::Return(true));

	EXPECT_CALL(
		dynamic_cast<const sic::MockFilter &>(childNodes.at(0)->getFilter()),
		evaluate(testing::Ref(assets.at(3))))
		.WillOnce(testing::Return(false));
	EXPECT_CALL(
		dynamic_cast<const sic::MockFilter &>(childNodes.at(1)->getFilter()),
		evaluate(testing::Ref(assets.at(3))))
		.WillOnce(testing::Return(false));
	EXPECT_CALL(
		dynamic_cast<const sic::MockFilter &>(childNodes.at(2)->getFilter()),
		evaluate(testing::Ref(assets.at(3))))
		.WillOnce(testing::Return(true));

	auto expValueTreeNodeMap =
		std::make_unique<sic::ValueTree::NodeWeightMap>();

	expValueTreeNodeMap->insert(std::pair(rootNode, 1));
	expValueTreeNodeMap->insert(std::pair(childNodes.at(0), 0.7));
	expValueTreeNodeMap->insert(std::pair(childNodes.at(1), 0.2));
	expValueTreeNodeMap->insert(std::pair(childNodes.at(2), 0.1));
	expValueTreeNodeMap->insert(std::pair(childNodes.at(3), 0.5));
	expValueTreeNodeMap->insert(std::pair(childNodes.at(4), 0.2));

	std::unique_ptr<sic::AbstractValueTree> expValueTree(
		new sic::ValueTree(std::move(expValueTreeNodeMap)));

	auto valueTree = validTree.evaluate(portfolio);

	ASSERT_TRUE(*valueTree == *expValueTree)
		<< "Generated ValueTree does not match expected.";
}

} // namespace
