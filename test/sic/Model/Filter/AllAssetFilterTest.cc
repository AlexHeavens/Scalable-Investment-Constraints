#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "sic/Model/Filter/AllAssetsFilter.hh"

#include "sic/Portfolio/AbstractAsset.hh"

namespace {

class AllAssetsFilterTest : public testing::Test {

public:
	class MockAsset : public sic::AbstractAsset {

	public:
		explicit MockAsset() : sic::AbstractAsset(1) {}

		MOCK_CONST_METHOD1(hasClass,
						   bool(sic::AbstractAsset::Class assetClass));
	};
};

TEST_F(AllAssetsFilterTest, CreateValidFilter) {
	const MockAsset assetA;
	const MockAsset assetB;
	const MockAsset assetC;
	const MockAsset assetD;

	sic::AllAssetsFilter validFilter;

	ASSERT_TRUE(validFilter.evaluate(assetA));
	ASSERT_TRUE(validFilter.evaluate(assetB));
	ASSERT_TRUE(validFilter.evaluate(assetC));
	ASSERT_TRUE(validFilter.evaluate(assetD));
}

} // namespace
