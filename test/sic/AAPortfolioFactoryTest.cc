#include <gtest/gtest.h>

#include <memory>
#include <vector>

#include "sic/AAPortfolioFactory.hh"
#include "sic/Base/Variable.hh"
#include "sic/Model/MockAssetAllocation.hh"
#include "sic/Model/MockAssetAllocationNode.hh"
#include "sic/Model/MockFilterNode.hh"
#include "sic/Portfolio/MockAsset.hh"
#include "sic/Portfolio/Position.hh"

namespace {

class AAPortfolioFactoryTest : public testing::Test {

public:
	sic::MockAssetAllocation aa;
	std::vector<const sic::MockAssetAllocationNode *> aaNodes;
	std::vector<std::unique_ptr<sic::MockFilterNode>> filterNodes;
	sic::AbstractAssetAllocation::FilterNodeMap filterNodeMap;
	std::unique_ptr<sic::AAPortfolioFactory> factory;
	std::vector<sic::MockAsset> aaAssets;
	std::vector<sic::Weight> aaAssetWeights;
	std::vector<sic::Value> expPositionValues;
	std::vector<sic::Position> expPositions;

	static constexpr int aaAssetCount = 5;
	static constexpr sic::Value expPortfolioRefValue = 123.00;
	static constexpr int aaNodeCount = 3;

	void SetUp() override {

		filterNodes.reserve(aaNodeCount);
		for (int i = 0; i < aaNodeCount; i++) {
			filterNodes.emplace_back(new sic::MockFilterNode());
		}

		aaNodes.reserve(aaNodeCount);
		for (int i = 0; i < aaNodeCount; i++) {
			const auto newAANodeRawPtr = new sic::MockAssetAllocationNode();
			std::unique_ptr<sic::AbstractAssetAllocationNode> newAANode(
				newAANodeRawPtr);
			filterNodeMap.insert(
				std::make_pair(filterNodes.at(i).get(), std::move(newAANode)));
			aaNodes.emplace_back(newAANodeRawPtr);
		}

		aaAssets.reserve(aaAssetCount);
		for (int i = 0; i < aaAssetCount; i++) {
			aaAssets.emplace_back();
		}

		aaAssetWeights.resize(aaAssetCount);

		aaAssetWeights.at(0) = 0.2;
		aaAssetWeights.at(1) = 0.4;
		aaAssetWeights.at(2) = 0.1;
		aaAssetWeights.at(3) = 0.1;
		aaAssetWeights.at(4) = 0.2;

		expPositionValues.resize(aaAssetCount);
		for (std::size_t positionIndex = 0; positionIndex < aaAssetCount;
			 positionIndex++) {
			expPositionValues.at(positionIndex) =
				aaAssetWeights.at(positionIndex) * expPortfolioRefValue;
		}

		for (int i = 0; i < aaAssetCount; i++) {
			const auto expValue = aaAssetWeights.at(i) * expPortfolioRefValue;
			const auto expID = static_cast<sic::External::ID>(i);
			expPositions.emplace_back(aaAssets.at(i), expValue, expID);
		}

		factory =
			std::make_unique<sic::AAPortfolioFactory>(aa, expPortfolioRefValue);
	}

	void addNode0Weights(sic::AbstractAsset::AssetWeightMap *assetWeightMap) {
		assetWeightMap->emplace(&aaAssets.at(0),
								sic::WeightRange(aaAssetWeights.at(0)));
		assetWeightMap->emplace(&aaAssets.at(3),
								sic::WeightRange(aaAssetWeights.at(3)));
		assetWeightMap->emplace(&aaAssets.at(4),
								sic::WeightRange(aaAssetWeights.at(4)));
	}
	void addNode2Weights(sic::AbstractAsset::AssetWeightMap *assetWeightMap) {
		assetWeightMap->emplace(&aaAssets.at(1),
								sic::WeightRange(aaAssetWeights.at(1)));
		assetWeightMap->emplace(&aaAssets.at(2),
								sic::WeightRange(aaAssetWeights.at(2)));
	}
};

constexpr int AAPortfolioFactoryTest::aaAssetCount;
constexpr sic::Value AAPortfolioFactoryTest::expPortfolioRefValue;
constexpr int AAPortfolioFactoryTest::aaNodeCount;

TEST_F(AAPortfolioFactoryTest, create) {

	sic::Iterators<sic::AbstractAssetAllocation::FilterNodeMap::value_type>
		nodeIts(filterNodeMap);
	EXPECT_CALL(aa, getAANodeIterators()).WillOnce(testing::Return(nodeIts));

	EXPECT_CALL(*aaNodes.at(0), attachToTopAssetWeights(testing::_))
		.WillOnce(
			testing::Invoke(this, &AAPortfolioFactoryTest::addNode0Weights));
	EXPECT_CALL(*aaNodes.at(1), attachToTopAssetWeights(testing::_))
		.WillOnce(testing::Return());
	EXPECT_CALL(*aaNodes.at(2), attachToTopAssetWeights(testing::_))
		.WillOnce(
			testing::Invoke(this, &AAPortfolioFactoryTest::addNode2Weights));

	auto portfolio = factory->create();

	ASSERT_EQ(portfolio->getPositionCount(), aaAssetCount)
		<< "Unexpected number of Positions in created Portfolio.  Should "
		   "match "
		   "Assets defined in AA.";

	auto posIts = portfolio->getPositionIterators();

	std::unordered_set<const sic::Position *> matchedPositions;
	while (posIts.remaining()) {
		const auto &position = *posIts.current();

		// Naive comparison of generator Portfolio Positions.
		bool matchingPosition = false;
		for (const auto &expPosition : expPositions) {
			if (matchedPositions.find(&expPosition) ==
					matchedPositions.end() and
				expPosition.getReferenceValue() ==
					position.getReferenceValue() and
				&expPosition.getAsset() == &position.getAsset()) {
				matchingPosition = true;
				matchedPositions.insert(&expPosition);
				break;
			}
		}

		ASSERT_TRUE(matchingPosition) << "Unexpected Position.";

		posIts.current()++;
	}
}

} // namespace
