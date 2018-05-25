#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "sic/Model/Filter.hh"

#include "sic/Portfolio/AbstractAsset.hh"

namespace {

class FilterTest : public testing::Test {

public:
	class MockAsset : public sic::AbstractAsset {

	public:
		explicit MockAsset() : sic::AbstractAsset(1) {}

		MOCK_CONST_METHOD1(hasClass, bool(sic::AbstractAsset::Class));
	};

	const static MockAsset assetA, assetB, assetC, assetD;

	class MockFilter : public sic::Filter {

	public:
		bool evaluate(const sic::AbstractAsset &asset) const override {
			return &asset == &assetA or &asset == &assetC;
		}
	};
};

const FilterTest::MockAsset FilterTest::assetA;
const FilterTest::MockAsset FilterTest::assetB;
const FilterTest::MockAsset FilterTest::assetC;
const FilterTest::MockAsset FilterTest::assetD;

TEST_F(FilterTest, CreateValidFilter) {
	MockFilter validFilter;

	ASSERT_TRUE(validFilter.evaluate(assetA));
	ASSERT_TRUE(validFilter.evaluate(assetC));

	ASSERT_FALSE(validFilter.evaluate(assetB));
	ASSERT_FALSE(validFilter.evaluate(assetD));
}

} // namespace
