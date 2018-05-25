#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "sic/Portfolio/AbstractAsset.hh"
#include "sic/Portfolio/Position.hh"

namespace {

class PositionTest : public testing::Test {
public:
	void SetUp() override {}
};

TEST_F(PositionTest, CreateValidPosition) {

	class MockAsset : public sic::AbstractAsset {

	public:
		MockAsset() : sic::AbstractAsset(1) {}

		MOCK_CONST_METHOD1(hasClass,
						   bool(sic::AbstractAsset::Class assetClass));
	};

	MockAsset asset;

	const sic::Value value = 333.33;
	const sic::External::ID expExternalID = 943875l;
	sic::Position validPosition(asset, value, expExternalID);

	ASSERT_EQ(&asset, &validPosition.getAsset());
	ASSERT_EQ(value, validPosition.getReferenceValue());
	ASSERT_EQ(expExternalID, validPosition.getExternalID());
}

} // namespace
