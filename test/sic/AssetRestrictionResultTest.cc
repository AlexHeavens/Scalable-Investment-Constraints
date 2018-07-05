#include <gtest/gtest.h>

#include "sic/AssetRestrictionResult.hh"
#include "sic/Portfolio/MockAsset.hh"

namespace {

class AssetRestrictionResultTest : public testing::Test {};

TEST_F(AssetRestrictionResultTest, ConstructPass) {

	const sic::External::ID assetID = 123;
	sic::MockAsset asset(assetID);
	const sic::Weight toTopWeight = 0.2;
	const sic::WeightRange weightRange(0.2, 0.3, 0.4);

	sic::AssetRestrictionResult assetResult(asset, toTopWeight, weightRange);

	std::string expSerialisation =
		"Asset,0,123,0.200000,0.200000,0.300000,0.400000\n";

	ASSERT_EQ(assetResult.getState(), sic::RestrictionResult::State::Pass);
	ASSERT_EQ(assetResult.serialise(), expSerialisation);
}

TEST_F(AssetRestrictionResultTest, ConstructFail) {

	const sic::External::ID assetID = 123;
	const sic::MockAsset asset(assetID);
	const sic::Weight toTopWeight = 0.199999;
	const sic::WeightRange weightRange(0.2, 0.3, 0.4);

	sic::AssetRestrictionResult assetResult(asset, toTopWeight, weightRange);

	std::string expSerialisation =
		"Asset,1,123,0.199999,0.200000,0.300000,0.400000\n";

	ASSERT_EQ(assetResult.getState(), sic::RestrictionResult::State::Fail);
	ASSERT_EQ(assetResult.serialise(), expSerialisation);
}

TEST_F(AssetRestrictionResultTest, Comparison) {

	const sic::MockAsset asset1, asset2;
	const sic::WeightRange weightRange1(0.1, 0.2, 0.3);
	const sic::WeightRange weightRange2(0.1, 0.21, 0.3);

	const sic::AssetRestrictionResult result1(asset1, 0.2, weightRange1);
	const sic::AssetRestrictionResult result2(asset1, 0.2, weightRange1);

	const sic::AssetRestrictionResult result3(asset2, 0.2, weightRange1);
	const sic::AssetRestrictionResult result4(asset1, 0.3, weightRange1);
	const sic::AssetRestrictionResult result5(asset1, 0.2, weightRange2);

	ASSERT_TRUE(result1 == result1);
	ASSERT_TRUE(result1 == result2);

	ASSERT_FALSE(result1 == result3);
	ASSERT_FALSE(result1 == result4);
	ASSERT_FALSE(result1 == result5);
}

} // namespace
