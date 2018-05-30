#include <gtest/gtest.h>

#include "sic/Model/Filter.hh"
#include "sic/Portfolio/MockAsset.hh"

namespace {

class FilterTest : public testing::Test {

public:
	const static sic::MockAsset assetA, assetB, assetC, assetD;

	class MockFilter : public sic::Filter {

	public:
		bool evaluate(const sic::AbstractAsset &asset) const override {
			return &asset == &assetA or &asset == &assetC;
		}
	};
};

const sic::MockAsset FilterTest::assetA;
const sic::MockAsset FilterTest::assetB;
const sic::MockAsset FilterTest::assetC;
const sic::MockAsset FilterTest::assetD;

TEST_F(FilterTest, CreateValidFilter) {
	MockFilter validFilter;

	ASSERT_TRUE(validFilter.evaluate(assetA));
	ASSERT_TRUE(validFilter.evaluate(assetC));

	ASSERT_FALSE(validFilter.evaluate(assetB));
	ASSERT_FALSE(validFilter.evaluate(assetD));
}

} // namespace
