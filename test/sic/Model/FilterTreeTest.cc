#include <gtest/gtest.h>

#include <memory>

#include "sic/Model/FilterTree.hh"
#include "sic/Model/MockFilter.hh"
#include "sic/Portfolio/MockAsset.hh"

namespace {

class FilterTreeTest : public testing::Test {};

TEST_F(FilterTreeTest, CreateValid) {

	const sic::External::ID expExternalID = 1;
	sic::FilterTree validTree(expExternalID);
}

TEST_F(FilterTreeTest, FilterAssetPath) {

	sic::MockAsset testAsset;

	const sic::External::ID expExternalID = 1;
	sic::FilterTree filterTree(expExternalID);

	std::vector<const sic::Filter *> filters;
	filters.reserve(5);
	filters[0] = new sic::MockFilter();
	filters[1] = new sic::MockFilter();
	filters[2] = new sic::MockFilter();
	filters[3] = new sic::MockFilter();
	filters[4] = new sic::MockFilter();

	std::vector<sic::AbstractFilterNode *> childNodes;
	sic::AbstractFilterNode &rootNode = filterTree.getRootNode();

	childNodes.reserve(5);
	childNodes[0] =
		&rootNode.addChild(std::unique_ptr<const sic::Filter>(filters[0]));
	childNodes[1] =
		&rootNode.addChild(std::unique_ptr<const sic::Filter>(filters[1]));
	childNodes[2] =
		&rootNode.addChild(std::unique_ptr<const sic::Filter>(filters[2]));
	childNodes[3] = &childNodes[0]->addChild(
		std::unique_ptr<const sic::Filter>(filters[3]));
	childNodes[4] = &childNodes[0]->addChild(
		std::unique_ptr<const sic::Filter>(filters[4]));

	EXPECT_CALL(
		dynamic_cast<const sic::MockFilter &>(childNodes[0]->getFilter()),
		evaluate(testing::Ref(testAsset)))
		.Times(1)
		.WillOnce(testing::Return(true));
	EXPECT_CALL(
		dynamic_cast<const sic::MockFilter &>(childNodes[3]->getFilter()),
		evaluate(testing::Ref(testAsset)))
		.Times(1)
		.WillOnce(testing::Return(false));
	EXPECT_CALL(
		dynamic_cast<const sic::MockFilter &>(childNodes[4]->getFilter()),
		evaluate(testing::Ref(testAsset)))
		.Times(1)
		.WillOnce(testing::Return(true));

	sic::FilterTree::Path path;
	filterTree.getAssetPath(testAsset, path);

	ASSERT_EQ(path.size(), 3);
	ASSERT_EQ(path[0], &(filterTree.getRootNode()));
	ASSERT_EQ(path[1], childNodes[0]);
	ASSERT_EQ(path[2], childNodes[4]);
}

} // namespace
