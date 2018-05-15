#include <gtest/gtest.h>

#include "sic/Model/FilterTree.hh"

namespace {

class FilterTreeTest : public testing::Test {

public:
	class MockFilterNode : public sic::Model::FilterNode {};
};

TEST_F(FilterTreeTest, CreateValid) {

	sic::Model::FilterTree validTree;

	// Doesn't do anything yet, besides hold the root node.
}

} // namespace
