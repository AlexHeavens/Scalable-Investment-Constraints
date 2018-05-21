#include <gtest/gtest.h>

#include <stdexcept>

#include "sic/Portfolio.hh"

namespace {

class PortfolioTest : public testing::Test {

private:
	// Mock Classes
	static constexpr sic::Price mockPrice = 100.00;
	static constexpr sic::External::ID externalAssetID = 43765l;

	class MockAsset : public sic::Asset {

	public:
		MockAsset() : sic::Asset(mockPrice, externalAssetID){};
	};

	static MockAsset mockAsset;
	static sic::External::ID externalPositionIDCounter;

public:
	class MockPosition : public sic::Position {

	public:
		explicit MockPosition()
			: sic::Position(mockAsset, 123.00, externalPositionIDCounter++) {}

		explicit MockPosition(sic::External::ID externalID)
			: sic::Position(mockAsset, 123.00, externalID) {}

		sic::Value getReferenceValue() const { return 123.00; }
	};

	void SetUp() override { externalPositionIDCounter = 43324l; }
};

PortfolioTest::MockAsset PortfolioTest::mockAsset;
sic::External::ID PortfolioTest::externalPositionIDCounter;

TEST_F(PortfolioTest, CreateValidPortfolio) {

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
		expPositions->push_back(MockPosition());
		expPositionAddresses.push_back(&expPositions->at(i));
	}

	// Create Portfolio
	constexpr sic::External::ID expExternalID = 34345l;

	sic::Portfolio validPortfolio(std::move(expPositions), expExternalID);

	// Check Portolio Positions
	auto positionIterators = validPortfolio.getPositionIterators();
	int expPositionIndex = 0;
	while (positionIterators.current() != positionIterators.end()) {

		ASSERT_TRUE(expPositionIndex < expPositionCount);

		ASSERT_EQ(expPositionAddresses.at(expPositionIndex),
				  &(*positionIterators.current()));
		expPositionIndex++;
		positionIterators.current()++;
	}
	ASSERT_EQ(expPositionCount, expPositionIndex);
	ASSERT_EQ(expExternalID, validPortfolio.getExternalID());
}

TEST_F(PortfolioTest, CreatePortfolioWithDuplicatePositions) {

	constexpr sic::External::ID expExternalID = 4345543l;
	constexpr sic::External::ID duplicatePositionExternalID = 234324l;
	MockPosition positionA(duplicatePositionExternalID);
	MockPosition positionB(23423535l);
	MockPosition positionC(duplicatePositionExternalID);

	std::unique_ptr<std::vector<sic::Position>> inputPositionVector(
		new std::vector<sic::Position>);
	inputPositionVector->push_back(positionA);
	inputPositionVector->push_back(positionB);
	inputPositionVector->push_back(positionC);

	const std::string expExceptionString = "Duplicate Portfolio Position ID";

	try {
		sic::Portfolio duplicatePositionPortfolio(
			std::move(inputPositionVector), expExternalID);
		FAIL() << "Able to create Portfolio with duplicate Position.";
	} catch (const std::invalid_argument &e) {
		ASSERT_EQ(expExceptionString, e.what());
	} catch (...) {
		FAIL() << "Unexpected  error."; // Unexpected exception type.
	}
}

} // namespace
