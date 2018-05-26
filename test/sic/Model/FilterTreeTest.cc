#include <gtest/gtest.h>

#include "sic/Model/FilterTree.hh"

namespace {

class FilterTreeTest : public testing::Test {

public:
	class MockFilterNode : public sic::FilterNode {};
};

TEST_F(FilterTreeTest, CreateValid) {

	const sic::External::ID expExternalID = 1;
	sic::FilterTree validTree(expExternalID);

	// Doesn't do anything yet, besides hold the root node.
}

} // namespace
