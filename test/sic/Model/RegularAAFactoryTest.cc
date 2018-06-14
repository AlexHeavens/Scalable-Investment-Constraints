#include <gtest/gtest.h>

#include <memory>

#include "sic/Base/Types.hh"
#include "sic/Model/MockFilterTree.hh"
#include "sic/Model/RegularAAFactory.hh"
#include "sic/Model/RegularFilterTreeFactory.hh"

namespace {

class RegularAAFactoryTest : public testing::Test {

public:
	static void checkAANode(const sic::AbstractAssetAllocation &aa,
							const sic::AbstractFilterTree &filterTree,
							const sic::AbstractFilterNode &node,
							const sic::Weight expParentWeight) {

		const int childCount = node.getChildCount();

		// The FilterTree is expected to be regular with one child node acting
		// as a catch-all.  If the current not has only the catch-all, or no
		// children, we have reached the leaves of the tree.
		if (childCount <= 1) {
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
		for (int i = 0; i < childCount - 1; i++) {

			const auto &childNode = *childIts.current();
			checkAANode(aa, filterTree, *childNode, weightPerNode);

			childIts.current()++;
		}
	}

	class MockFilterTreeSource
		: public sic::Source<std::unique_ptr<sic::AbstractFilterTree>> {

	public:
		MOCK_METHOD0(
			getItems,
			sic::Iterators<std::unique_ptr<sic::AbstractFilterTree>>());
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

	sic::Iterators<std::unique_ptr<sic::AbstractFilterTree>>::It begin(
		filterTrees.begin());
	sic::Iterators<std::unique_ptr<sic::AbstractFilterTree>>::It end(
		filterTrees.end());
	sic::Iterators<std::unique_ptr<sic::AbstractFilterTree>> filterTreesIt(
		begin, end);
	MockFilterTreeSource filterTreeSource;
	EXPECT_CALL(filterTreeSource, getItems())
		.WillOnce(testing::Return(filterTreesIt))
		.WillOnce(testing::Return(filterTreesIt));

	sic::RegularAAFactory factory(filterTreeSource);

	constexpr int expAACount = 6;
	for (int i = 0; i < expAACount; i++) {

		auto newAA = factory.create();

		// FilterTrees are expected to be used in a round-robin fashion.
		const int expAAIndex = i % filterTreeCount;
		const auto &expFilterTree = *filterTrees.at(expAAIndex);
		ASSERT_EQ(&newAA->getFilterTree(), &expFilterTree)
			<< "AA created with unexpected FilterTree.";

		const auto &rootNode = expFilterTree.getRootNode();
		const auto &rootAANode = newAA->getAANode(rootNode);
		const auto &rootWeights = rootAANode.getWeightRange();

		ASSERT_EQ(rootWeights.target, 1.0)
			<< "Root node has unexpected target weight.";

		checkAANode(*newAA, expFilterTree, rootNode, 1.0);
	}
}

} // namespace
