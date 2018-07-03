#include <gtest/gtest.h>

#include <memory>

#include "sic/Base/Types.hh"
#include "sic/Model/MPFAllocationNode.hh"
#include "sic/Model/MockFilterTree.hh"
#include "sic/Model/RegularAAFactory.hh"
#include "sic/Model/RegularFilterTreeFactory.hh"
#include "sic/Portfolio/Asset.hh"

namespace {

class RegularAAFactoryTest : public testing::Test {

public:
	static void checkAANode(const sic::AbstractAssetAllocation &aa,
							const sic::AbstractFilterTree &filterTree,
							const sic::AbstractFilterNode &node,
							const sic::Weight expParentWeight,
							const int expRemainingDepth,
							const int expTreeNodeDegree,
							const bool expMPFNode) {

		const int childCount = node.getChildCount();

		// The FilterTree is expected to be regular with one child node acting
		// as a catch-all.  If the current not has only the catch-all, or no
		// children, we have reached the leaves of the tree.
		const auto expNodeDegree =
			(expRemainingDepth == 1) ? 0 : expTreeNodeDegree;

		const auto &aaNode = aa.getAANode(node);

		// Leaf nodes should be MPFs.
		if (expRemainingDepth == 1 and expMPFNode) {
			ASSERT_EQ(typeid(aaNode), typeid(sic::MPFAllocationNode))
				<< "Expected MPF AA Node with remaining depth 1.";
		} else {
			ASSERT_EQ(typeid(aaNode), typeid(sic::AssetAllocationNode))
				<< "Expected standard AA Node with remaining depth "
				<< expRemainingDepth << ".";
		}

		if (childCount != expNodeDegree or childCount == 0) {
			return;
		}

		const sic::Weight weightPerNode =
			expParentWeight / static_cast<sic::Weight>(childCount - 1);

		auto childIts = node.getChildIterators();
		for (int i = 0; i < childCount - 1; i++) {

			const auto &childNode = **childIts.current();
			const auto &childAANode = aa.getAANode(childNode);
			const auto &childWeights = childAANode.getWeightRange();

			ASSERT_EQ(childWeights.target, weightPerNode)
				<< "Child node does not have expected target weight.";

			childIts.current()++;
		}

		const auto &childNode = **childIts.current();
		const auto &childAANode = aa.getAANode(childNode);
		const auto &childWeights = childAANode.getWeightRange();

		ASSERT_EQ(childWeights.target, 0.0)
			<< "Catch all node does not have 0 target weight.";

		childIts = node.getChildIterators();
		for (int i = 0; i < childCount; i++) {

			const auto &childNode = *childIts.current();
			const auto childDepth = expRemainingDepth - 1;
			const auto expMPFNode = (childDepth == 1 and i < childCount - 1);

			checkAANode(aa, filterTree, *childNode, weightPerNode, childDepth,
						expNodeDegree, expMPFNode);

			childIts.current()++;
		}
	}

	class MockFilterTreeSource
		: public sic::Source<std::unique_ptr<sic::AbstractFilterTree>> {

	public:
		MOCK_CONST_METHOD0(
			getItems,
			sic::Iterators<std::unique_ptr<sic::AbstractFilterTree>>());
		MOCK_CONST_METHOD0(size, std::size_t());
	};
};

TEST_F(RegularAAFactoryTest, CreateValid) {

	constexpr unsigned nodeDegree = 3;
	constexpr unsigned treeDepth = 3;
	constexpr sic::External::ID filterTreeID = 1;
	sic::RegularFilterTreeFactory filterTreeFactory(treeDepth, nodeDegree);

	constexpr int filterTreeCount = 3;
	std::vector<std::unique_ptr<sic::AbstractFilterTree>> filterTrees;
	for (int i = 0; i < filterTreeCount; i++) {
		auto treePtr = filterTreeFactory.create(filterTreeID);
		filterTrees.push_back(std::move(treePtr));
	}

	sic::Iterators<std::unique_ptr<sic::AbstractFilterTree>> filterTreesIt(
		filterTrees);
	MockFilterTreeSource filterTreeSource;
	EXPECT_CALL(filterTreeSource, getItems())
		.WillOnce(testing::Return(filterTreesIt))
		.WillOnce(testing::Return(filterTreesIt));

	std::vector<std::unique_ptr<sic::AbstractAsset>> assetSource;

	// 100 class groups, up to 4 classes per group.
	constexpr unsigned classGroupCount = 100;
	constexpr unsigned groupJump = 100;
	constexpr unsigned classesPerGroup = 4;

	// Give each Asset a random class from each group.
	std::default_random_engine randomEngine;
	std::uniform_int_distribution<> classDistribution(0, classesPerGroup - 1);

	constexpr sic::External::ID assetIDsFrom = 1000;
	constexpr unsigned assetCount = 500;
	for (sic::Asset::ID assetID = assetIDsFrom;
		 assetID < assetIDsFrom + assetCount; assetID++) {

		std::unique_ptr<sic::AbstractAsset::ClassSet> assetClasses(
			new sic::AbstractAsset::ClassSet);
		for (unsigned classGroup = 0; classGroup < classGroupCount;
			 classGroup++) {
			const sic::Asset::Class groupClass =
				classGroup * groupJump + classDistribution(randomEngine);
			assetClasses->insert(groupClass);
		}

		assetSource.emplace_back(
			new sic::Asset(assetID, std::move(assetClasses)));
	}

	constexpr size_t expMPFAssetCount = 8;
	constexpr sic::External::ID expInitialAAID = 123;
	sic::RegularAAFactory factory(filterTreeSource, assetSource,
								  expMPFAssetCount, expInitialAAID);

	constexpr int expAACount = 6;
	for (int i = 0; i < expAACount; i++) {

		auto newAAMPFs = factory.create();
		auto &newAA = newAAMPFs.first;

		// Check AAs.
		ASSERT_EQ(newAA->getExternalID(), expInitialAAID + i)
			<< "AA does not have expected ID.";

		// FilterTrees are expected to be used in a round-robin fashion.
		const auto expAAIndex = i % filterTreeCount;
		const auto &expFilterTree = *filterTrees.at(expAAIndex);
		ASSERT_EQ(&newAA->getFilterTree(), &expFilterTree)
			<< "AA created with unexpected FilterTree.";

		const auto &rootNode = expFilterTree.getRootNode();
		const auto &rootAANode = newAA->getAANode(rootNode);
		const auto &rootWeights = rootAANode.getWeightRange();

		ASSERT_EQ(rootWeights.target, 1.0)
			<< "Root node has unexpected target weight.";

		checkAANode(*newAA, expFilterTree, rootNode, 1.0, treeDepth, nodeDegree,
					false);

		// Check MPFs.
		auto &newMPFs = *(newAAMPFs.second);

		// Only expecting MPFs for non catch-all nodes.
		const auto expMPFCount = 4;
		ASSERT_EQ(newMPFs.size(), expMPFCount)
			<< "Unexpected generate MPF count.";

		for (std::size_t i = 0; i < expMPFCount; i++) {

			auto &mpf = *(newMPFs.at(i));
			ASSERT_TRUE(mpf.getAssetCount() == expMPFAssetCount)
				<< "Unexpected number of assets in MPF " << i << ".";
		}
	}
}

} // namespace
