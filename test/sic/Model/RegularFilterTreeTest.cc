#include <gtest/gtest.h>

#include "sic/Model/RegularFilterTreeFactory.hh"

namespace {

class RegularFilterTreeFactoryTest : public testing::Test {

public:
	static void assertNode(const sic::AbstractFilterNode &node,
						   unsigned expMaxDepth, unsigned nodeDepth,
						   unsigned expNodeDegree) {

		if (nodeDepth == expMaxDepth) {
			ASSERT_EQ(node.getChildCount(), 0);
		} else {

			ASSERT_EQ(node.getChildCount(), expNodeDegree);

			auto children = node.getChildIterators();
			while (children.current() != children.end()) {
				assertNode(**(children.current()), expMaxDepth, nodeDepth + 1,
						   expNodeDegree);
				children.current()++;
			}
		}
	}
};

TEST_F(RegularFilterTreeFactoryTest, CreateValid) {

	constexpr unsigned expDepth = 3;
	constexpr unsigned expNodeDegree = 3;

	sic::RegularFilterTreeFactory factory(expDepth, expNodeDegree);

	const sic::External::ID filterTreeID = 123;
	sic::FilterTree filterTree(filterTreeID);

	factory.create(filterTree);

	auto &rootNode = filterTree.getRootNode();
	RegularFilterTreeFactoryTest::assertNode(rootNode, expDepth, 1,
											 expNodeDegree);
}

TEST_F(RegularFilterTreeFactoryTest, getPathClasses) {

	constexpr unsigned expDepth = 4;
	constexpr unsigned expNodeDegree = 3;

	sic::RegularFilterTreeFactory factory(expDepth, expNodeDegree);

	// Simple, left-most path.
	const std::vector<unsigned> path1Children = {0, 0, 0};
	auto path1ClassSet = factory.getPathClasses(path1Children);

	ASSERT_EQ(path1ClassSet->size(), 3);
	ASSERT_EQ(path1ClassSet->count(0), 1);
	ASSERT_EQ(path1ClassSet->count(100), 1);
	ASSERT_EQ(path1ClassSet->count(300), 1);

	// More complex path.
	const std::vector<unsigned> path2Children = {1, 0, 1};
	auto path2ClassSet = factory.getPathClasses(path2Children);

	ASSERT_EQ(path2ClassSet->size(), 3);
	ASSERT_EQ(path2ClassSet->count(1), 1);
	ASSERT_EQ(path2ClassSet->count(200), 1);
	ASSERT_EQ(path2ClassSet->count(501), 1);
}

} // namespace
