#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "sic/Model/Filter/AssetClassFilter.hh"

#include "sic/Portfolio/AbstractAsset.hh"

namespace {

class AssetsClassFilterTest : public testing::Test {

public:
	class MockAsset : public sic::AbstractAsset {

	public:
		explicit MockAsset() : sic::AbstractAsset(1) {}

		MOCK_CONST_METHOD1(hasClass,
						   bool(sic::AbstractAsset::Class assetClass));
	};
};

TEST_F(AssetsClassFilterTest, CreateValidFilter) {
	const MockAsset assetA;
	const MockAsset assetB;
	const MockAsset assetC;
	const MockAsset assetD;

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

	sic::Model::AssetClassFilter validFilter(assetClass);

	ASSERT_TRUE(validFilter.evaluate(assetA));
	ASSERT_TRUE(validFilter.evaluate(assetB));
	ASSERT_FALSE(validFilter.evaluate(assetC));
	ASSERT_FALSE(validFilter.evaluate(assetD));
}

} // namespace
