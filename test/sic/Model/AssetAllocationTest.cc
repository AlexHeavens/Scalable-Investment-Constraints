#include <gtest/gtest.h>

#include "sic/AANodeRestrictionResult.hh"
#include "sic/Base/Variable.hh"
#include "sic/MockPortfolio.hh"
#include "sic/Model/AssetAllocation.hh"
#include "sic/Model/MockAssetAllocationNode.hh"
#include "sic/Model/MockFilterNode.hh"
#include "sic/Model/MockFilterTree.hh"
#include "sic/Portfolio/MockValueTree.hh"

namespace {

class AssetAllocationTest : public testing::Test {
public:
	static constexpr int filterNodeCount = 6;
	sic::MockFilterTree filterTree;
	sic::MockPortfolio portfolio;
	std::vector<std::unique_ptr<sic::MockFilterNode>> filterNodes;
	std::vector<sic::WeightRange> aaNodeWeights;

	void SetUp() override {

		filterNodes.reserve(filterNodeCount);

		for (int i = 0; i < filterNodeCount; i++) {
			filterNodes.emplace_back(new sic::MockFilterNode);
		}
	}
};

constexpr int AssetAllocationTest::filterNodeCount;

TEST_F(AssetAllocationTest, CreateValid) {

	std::vector<std::unique_ptr<sic::MockAssetAllocationNode>> aaNodes;
	aaNodes.reserve(filterNodeCount);

	for (int i = 0; i < filterNodeCount; i++) {
		aaNodes.emplace_back(new sic::MockAssetAllocationNode());
	}

	for (int i = 0; i < filterNodeCount; i++) {
		EXPECT_CALL(*filterNodes.at(i), getFilterTree())
			.WillOnce(testing::ReturnRef(filterTree));
	}

	auto filterNodeMap =
		std::make_unique<sic::AssetAllocation::FilterNodeMap>();

	for (int i = 0; i < filterNodeCount; i++) {
		filterNodeMap->insert(
			std::make_pair(filterNodes.at(i).get(), std::move(aaNodes.at(i))));
	}

	constexpr sic::External::ID aaID = 1;

	sic::AssetAllocation validAA(filterTree, std::move(filterNodeMap), aaID);

	ASSERT_EQ(&validAA.getFilterTree(), &filterTree);
}

TEST_F(AssetAllocationTest, EmptyNodeMap) {

	const sic::MockFilterTree filterTree;
	std::unique_ptr<sic::AssetAllocation::FilterNodeMap> nullPtrFilterNodeMap;

	constexpr sic::External::ID aaID = 1;

	const std::string expExceptionString = "empty filterNodeMap.";
	try {
		sic::AssetAllocation aa(filterTree, std::move(nullPtrFilterNodeMap),
								aaID);
		unused(aa);

		FAIL()
			<< "Able to pass filter tree node with null pointer filterNodeMap.";
	} catch (const std::invalid_argument &e) {
		ASSERT_EQ(expExceptionString, e.what());
	} catch (const std::exception &e) {
		FAIL() << "Unexpected  error: " << e.what() << "\n";
	}

	auto emptyFilterNodeMap =
		std::make_unique<sic::AssetAllocation::FilterNodeMap>();

	try {
		sic::AssetAllocation aa(filterTree, std::move(emptyFilterNodeMap),
								aaID);
		unused(aa);

		FAIL() << "Able to pass filter tree node with empty filterNodeMap.";
	} catch (const std::invalid_argument &e) {
		ASSERT_EQ(expExceptionString, e.what());
	} catch (const std::exception &e) {
		FAIL() << "Unexpected  error: " << e.what() << "\n";
	}
}

TEST_F(AssetAllocationTest, MapUnknownFilterTreeNode) {

	testing::NiceMock<sic::MockFilterNode> knownNode1, knownNode2;
	sic::MockFilterNode unknownNode;
	const sic::MockFilterTree filterTree;
	const sic::MockFilterTree unknownFilterTree;

	auto aaNode1 = std::make_unique<sic::MockAssetAllocationNode>();
	auto aaNode2 = std::make_unique<sic::MockAssetAllocationNode>();
	auto aaNode3 = std::make_unique<sic::MockAssetAllocationNode>();

	auto filterNodeMap =
		std::make_unique<sic::AssetAllocation::FilterNodeMap>();
	filterNodeMap->insert(std::make_pair(&knownNode1, std::move(aaNode1)));
	filterNodeMap->insert(std::make_pair(&unknownNode, std::move(aaNode2)));
	filterNodeMap->insert(std::make_pair(&knownNode2, std::move(aaNode3)));

	// Difficult to predict path through node map, so may or may not query other
	// nodes for their filter tree.
	ON_CALL(knownNode1, getFilterTree())
		.WillByDefault(testing::ReturnRef(filterTree));
	ON_CALL(knownNode2, getFilterTree())
		.WillByDefault(testing::ReturnRef(filterTree));

	EXPECT_CALL(unknownNode, getFilterTree())
		.WillOnce(testing::ReturnRef(unknownFilterTree));

	constexpr sic::External::ID aaID = 1;

	const std::string expExceptionString =
		"filterNodeMap contains FilterNode from an unknown tree.";
	try {
		sic::AssetAllocation aa(filterTree, std::move(filterNodeMap), aaID);
		unused(aa);

		FAIL() << "Able to pass filter tree node from unknown tree.";
	} catch (const std::invalid_argument &e) {
		ASSERT_EQ(expExceptionString, e.what());
	} catch (const std::exception &e) {
		FAIL() << "Unexpected  error: " << e.what() << "\n";
	}
}

TEST_F(AssetAllocationTest, generateRestrictionResults) {

	for (int i = 0; i < filterNodeCount; i++) {
		EXPECT_CALL(*filterNodes.at(i), getFilterTree())
			.WillOnce(testing::ReturnRef(filterTree));
	}

	std::vector<std::unique_ptr<sic::MockAssetAllocationNode>> aaNodes;
	std::vector<sic::MockAssetAllocationNode *> aaNodePtrs;
	aaNodes.reserve(filterNodeCount);
	aaNodePtrs.reserve(filterNodeCount);
	for (int i = 0; i < filterNodeCount; i++) {
		aaNodePtrs.push_back(new sic::MockAssetAllocationNode());
		aaNodes.emplace_back(aaNodePtrs.at(i));
	}

	auto filterNodeMap =
		std::make_unique<sic::AssetAllocation::FilterNodeMap>();
	for (int i = 0; i < filterNodeCount; i++) {
		filterNodeMap->insert(
			std::make_pair(filterNodes.at(i).get(), std::move(aaNodes.at(i))));
	}

	// As GMock can't handle mocking unique_ptr, a wrapping function takes a
	// new MockValueTree and will add it to a unique_ptr.
	auto *valueTreeRawPtr = new sic::MockValueTree();
	auto &valueTree = *valueTreeRawPtr;
	EXPECT_CALL(filterTree, evaluateRaw(testing::Ref(portfolio)))
		.WillOnce(testing::Return(valueTreeRawPtr));

	sic::AbstractValueTree::NodeWeightMap valueTreeWeights;

	valueTreeWeights.insert(std::make_pair(filterNodes.at(0).get(), 1.0));
	valueTreeWeights.insert(std::make_pair(filterNodes.at(1).get(), 0.51));
	valueTreeWeights.insert(std::make_pair(filterNodes.at(2).get(), 0.19));
	valueTreeWeights.insert(std::make_pair(filterNodes.at(3).get(), 0.3));
	valueTreeWeights.insert(std::make_pair(filterNodes.at(4).get(), 0.01));
	valueTreeWeights.insert(std::make_pair(filterNodes.at(5).get(), 0.5));

	sic::Iterators<sic::AbstractValueTree::NodeWeightMap::value_type>
		nodeWeightIts(valueTreeWeights);
	EXPECT_CALL(valueTree, getNodeWeightIterators())
		.WillOnce(testing::Return(nodeWeightIts));

	std::vector<sic::AANodeRestrictionResult> expResults;
	expResults.reserve(filterNodeCount);
	for (int i = 0; i < filterNodeCount; i++) {
		const auto &filterNode = *filterNodes.at(i);
		const auto &expValueTreePair = *valueTreeWeights.find(&filterNode);
		const auto expNodeWeight = expValueTreePair.second;
		expResults.emplace_back(*aaNodePtrs.at(i), expNodeWeight);
	}

	constexpr sic::External::ID aaID = 123;
	sic::AssetAllocation aa(filterTree, std::move(filterNodeMap), aaID);

	const auto results = aa.generateRestrictionResults(portfolio);

	ASSERT_EQ(results->size(), filterNodeCount);

	// Naive check that results match.
	std::unordered_set<const sic::AANodeRestrictionResult *> matchedResults;
	for (const auto &resultPtr : *results) {
		const auto &result = *resultPtr;

		for (const auto &expResult : expResults) {

			if (matchedResults.find(&expResult) == matchedResults.end() and
				expResult == result) {
				matchedResults.insert(&expResult);
				break;
			}
		}
	}

	ASSERT_EQ(matchedResults.size(), filterNodeCount)
		<< "Not all results match expected.";
}

} // namespace
