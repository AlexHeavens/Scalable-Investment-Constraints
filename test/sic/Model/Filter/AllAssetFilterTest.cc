#include <gtest/gtest.h>

#include "sic/Model/Filter/AllAssetsFilter.hh"
#include "sic/Portfolio/MockAsset.hh"

namespace {

class AllAssetsFilterTest : public testing::Test {};

TEST_F(AllAssetsFilterTest, CreateValidFilter) {

	const sic::MockAsset assetA;
	const sic::MockAsset assetB;
	const sic::MockAsset assetC;
	const sic::MockAsset assetD;

	sic::AllAssetsFilter validFilter;

	ASSERT_TRUE(validFilter.evaluate(assetA));
	ASSERT_TRUE(validFilter.evaluate(assetB));
	ASSERT_TRUE(validFilter.evaluate(assetC));
	ASSERT_TRUE(validFilter.evaluate(assetD));
}

} // namespace
