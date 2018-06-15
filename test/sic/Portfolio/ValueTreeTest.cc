#include <gtest/gtest.h>

#include <vector>

#include "sic/Model/MockFilterNode.hh"
#include "sic/Portfolio/ValueTree.hh"

namespace {

class ValueTreeTest : public testing::Test {};

TEST_F(ValueTreeTest, CreateValid) {

	const int filterNodeCount = 10;
	std::vector<std::unique_ptr<sic::MockFilterNode>> filterNodes;

	filterNodes.reserve(filterNodeCount);
	for (int i = 0; i < filterNodeCount; i++) {
		filterNodes.emplace_back(new sic::MockFilterNode());
	}

	auto nodeWeightMap = std::make_unique<sic::ValueTree::NodeWeightMap>();
	for (int i = 0; i < filterNodeCount; i++) {
		nodeWeightMap->insert(std::pair(filterNodes.at(i).get(), 0.1));
	}

	sic::ValueTree valueTree(std::move(nodeWeightMap));
}

} // namespace
