#include <gtest/gtest.h>

#include "sic/Portfolio.hh"

namespace {

class PortfolioTest : public testing::Test {
public:
	void SetUp() override {}
};

TEST_F(PortfolioTest, CreateValidPortfolio) {

	// Mock Classes
	static constexpr sic::Price mockPrice = 100.00;
	static constexpr sic::External::ID externalAssetID = 43765l;

	class MockAsset : public sic::Asset {

	public:
		MockAsset() : sic::Asset(mockPrice, externalAssetID){};
	};

	static MockAsset mockAsset;
	static sic::External::ID externalPositionIDCounter = 43765l;

	class MockPosition : public sic::Position {

	public:
		explicit MockPosition(sic::Value referenceValue)
			: sic::Position(mockAsset, referenceValue,
							externalPositionIDCounter++) {}

		sic::Value getReferenceValue() const { return 123.00; }
	};

	// Mock Positions
	static constexpr int expPositionCount = 500;
	std::unique_ptr<std::vector<sic::Position>> expPositions(
		new std::vector<sic::Position>);
	std::vector<sic::Position *> expPositionAddresses;

	// Store the addresses of the mock positions to compare after
	// intiialisation.  Warning: must be careful to reserve vector space to
	// avoid re-addressing due to dynamic vector expansion.
	expPositions->reserve(expPositionCount);
	expPositionAddresses.reserve(expPositionCount);

	for (int i = 0; i < expPositionCount; i++) {
		expPositions->push_back(MockPosition(123.00));
		expPositionAddresses.push_back(&expPositions->at(i));
	}

	// Create Portfolio
	constexpr sic::External::ID expExternalID = 34345l;

	sic::Portfolio validPortfolio(std::move(expPositions), expExternalID);

	// Check Portolio Positions
	auto positionIterators = validPortfolio.getPositionIterators();
	int expPositionIndex = 0;
	while (positionIterators.first != positionIterators.second) {

		ASSERT_TRUE(expPositionIndex < expPositionCount);

		ASSERT_EQ(expPositionAddresses.at(expPositionIndex),
				  &(*positionIterators.first));
		expPositionIndex++;
		positionIterators.first++;
	}
	ASSERT_EQ(expPositionCount, expPositionIndex);
	ASSERT_EQ(expExternalID, validPortfolio.getExternalID());
}

} // namespace
