#include <gtest/gtest.h>

#include <memory>
#include <vector>

#include "sic/AAPortfolioFactory.hh"
#include "sic/Model/MockAssetAllocation.hh"
#include "sic/Portfolio/MockAsset.hh"
#include "sic/Portfolio/Position.hh"

namespace {

class AAPortfolioFactoryTest : public testing::Test {

public:
	sic::MockAssetAllocation aa;
	std::vector<sic::MockAsset> aaAssets;
	std::vector<sic::Weight> aaAssetWeights;
	std::vector<sic::Value> expPositionValues;
	std::vector<sic::Position> expPositions;

	static constexpr int aaAssetCount = 5;
	static constexpr sic::Value expPortfolioRefValue = 123.00;

	void SetUp() override {

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
	}
};

constexpr int AAPortfolioFactoryTest::aaAssetCount;
constexpr sic::Value AAPortfolioFactoryTest::expPortfolioRefValue;

TEST_F(AAPortfolioFactoryTest, create) {

	auto assetWeightMap = new sic::AbstractAsset::AssetWeightMap();
	assetWeightMap->emplace(&aaAssets.at(0),
							sic::WeightRange(aaAssetWeights.at(0)));
	assetWeightMap->emplace(&aaAssets.at(3),
							sic::WeightRange(aaAssetWeights.at(3)));
	assetWeightMap->emplace(&aaAssets.at(4),
							sic::WeightRange(aaAssetWeights.at(4)));
	assetWeightMap->emplace(&aaAssets.at(1),
							sic::WeightRange(aaAssetWeights.at(1)));
	assetWeightMap->emplace(&aaAssets.at(2),
							sic::WeightRange(aaAssetWeights.at(2)));

	EXPECT_CALL(aa, getAssetToTopWeightsRaw())
		.WillOnce(testing::Return(assetWeightMap));

	sic::AAPortfolioFactory factory(aa, expPortfolioRefValue);

	auto portfolio = factory.create();

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
