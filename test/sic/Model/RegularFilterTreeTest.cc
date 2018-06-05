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

} // namespace
