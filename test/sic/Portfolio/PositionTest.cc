#include <gtest/gtest.h>

#include "sic/Portfolio/Asset.hh"
#include "sic/Portfolio/Position.hh"

namespace {

class PositionTest : public testing::Test {
public:
	void SetUp() override {}
};

TEST_F(PositionTest, CreateValidPosition) {

	static const sic::Price price = 100.00;
	static const sic::External::ID externalAssetID = 43985543l;

	class MockAsset : public sic::Asset {

	public:
		explicit MockAsset(sic::Price price, sic::External::ID externalID)
			: sic::Asset(price, externalID) {}

		sic::Price getReferencePrice() const { return price; }
	};

	MockAsset asset(price, externalAssetID);

	const sic::Value value = 333.33;
	const sic::External::ID expExternalID = 943875l;
	sic::Position validPosition(asset, value, expExternalID);

	ASSERT_EQ(&asset, &validPosition.getAsset());
	ASSERT_EQ(value, validPosition.getReferenceValue());
	ASSERT_EQ(expExternalID, validPosition.getExternalID());
}

} // namespace
