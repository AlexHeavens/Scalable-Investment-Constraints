#include <gtest/gtest.h>

#include "sic/Model/Filter/AllAssetsFilter.hh"

#include "sic/Portfolio/Asset.hh"

namespace {

class AllAssetsFilterTest : public testing::Test {

public:
	class MockAsset : public sic::Asset {

	public:
		explicit MockAsset() : sic::Asset(123.00, 3434l) {}
	};
};

TEST_F(AllAssetsFilterTest, CreateValidFilter) {
	const AllAssetsFilterTest::MockAsset assetA;
	const AllAssetsFilterTest::MockAsset assetB;
	const AllAssetsFilterTest::MockAsset assetC;
	const AllAssetsFilterTest::MockAsset assetD;

	sic::Model::AllAssetsFilter validFilter;

	ASSERT_TRUE(validFilter.evaluate(assetA));
	ASSERT_TRUE(validFilter.evaluate(assetB));
	ASSERT_TRUE(validFilter.evaluate(assetC));
	ASSERT_TRUE(validFilter.evaluate(assetD));
}

} // namespace
