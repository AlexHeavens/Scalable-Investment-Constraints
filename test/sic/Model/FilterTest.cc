#include <gtest/gtest.h>

#include "sic/Model/Filter.hh"

#include "sic/Portfolio/Asset.hh"

namespace {

class FilterTest : public testing::Test {

public:
	class MockAsset : public sic::Asset {

	public:
		explicit MockAsset() : sic::Asset(123.00, 3434l) {}
	};

	const static FilterTest::MockAsset assetA, assetB, assetC, assetD;

	static bool passAssetsAC(const sic::Asset &asset) {
		return &asset == &assetA or &asset == &assetC;
	}
};

const FilterTest::MockAsset FilterTest::assetA;
const FilterTest::MockAsset FilterTest::assetB;
const FilterTest::MockAsset FilterTest::assetC;
const FilterTest::MockAsset FilterTest::assetD;

TEST_F(FilterTest, CreateValidFilter) {
	sic::Model::Filter validFilter(FilterTest::passAssetsAC);

	ASSERT_TRUE(validFilter.evaluate(assetA));
	ASSERT_TRUE(validFilter.evaluate(assetC));

	ASSERT_FALSE(validFilter.evaluate(assetB));
	ASSERT_FALSE(validFilter.evaluate(assetD));
}

} // namespace
