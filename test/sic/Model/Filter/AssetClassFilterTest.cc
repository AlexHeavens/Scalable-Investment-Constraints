#include <gtest/gtest.h>

#include "sic/Model/Filter/AssetClassFilter.hh"
#include "sic/Portfolio/MockAsset.hh"

namespace {

class AssetsClassFilterTest : public testing::Test {};

TEST_F(AssetsClassFilterTest, CreateValidFilter) {
	const sic::MockAsset assetA;
	const sic::MockAsset assetB;
	const sic::MockAsset assetC;
	const sic::MockAsset assetD;

	const sic::AbstractAsset::Class assetClass = 12345;
	EXPECT_CALL(assetA, hasClass(assetClass))
		.Times(1)
		.WillOnce(testing::Return(true));
	EXPECT_CALL(assetB, hasClass(assetClass))
		.Times(1)
		.WillOnce(testing::Return(true));
	EXPECT_CALL(assetC, hasClass(assetClass))
		.Times(1)
		.WillOnce(testing::Return(false));
	EXPECT_CALL(assetD, hasClass(assetClass))
		.Times(1)
		.WillOnce(testing::Return(false));

	sic::AssetClassFilter validFilter(assetClass);

	ASSERT_TRUE(validFilter.evaluate(assetA));
	ASSERT_TRUE(validFilter.evaluate(assetB));
	ASSERT_FALSE(validFilter.evaluate(assetC));
	ASSERT_FALSE(validFilter.evaluate(assetD));
}

} // namespace
