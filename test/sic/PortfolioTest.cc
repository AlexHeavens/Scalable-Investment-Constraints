#include <gtest/gtest.h>

#include <stdexcept>

#include "sic/Portfolio.hh"
#include "sic/Portfolio/MockAsset.hh"
#include "sic/Portfolio/MockPosition.hh"

namespace {

class PortfolioTest : public testing::Test {};

TEST_F(PortfolioTest, CreateValidPortfolio) {

	// Mock Positions
	static constexpr int expPositionCount = 500;
	auto expPositions = std::make_unique<std::vector<sic::MockPosition>>();
	std::vector<const sic::MockPosition *> expPositionAddresses;

	// Store the addresses of the mock positions to compare after
	// intiialisation.  Warning: must be careful to reserve vector space to
	// avoid re-addressing due to dynamic vector expansion.
	expPositions->reserve(expPositionCount);
	expPositionAddresses.reserve(expPositionCount);

	sic::Value expTotalValue = 0.0;
	sic::External::ID nextPositionID = 123;
	for (int i = 0; i < expPositionCount; i++) {
		expPositions->emplace_back(nextPositionID++);

		const auto &newPosition = expPositions->at(i);
		expPositionAddresses.push_back(&newPosition);

		EXPECT_CALL(newPosition, getReferenceValue())
			.WillOnce(testing::Return(0.3));
		expTotalValue += 0.3;
	}

	// Create Portfolio
	constexpr sic::External::ID expExternalID = 34345l;

	sic::Portfolio<sic::MockPosition> validPortfolio(std::move(expPositions),
													 expExternalID);

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
	ASSERT_EQ(expTotalValue, validPortfolio.getTotalReferenceValue());
}

TEST_F(PortfolioTest, CreatePortfolioWithDuplicatePositions) {

	constexpr sic::External::ID expExternalID = 4345543l;
	constexpr sic::External::ID duplicatePositionExternalID = 234324l;

	auto positions = std::make_unique<std::vector<sic::MockPosition>>();
	const auto &positionsPtr = positions.get();

	constexpr int positionCount = 3;
	positions->reserve(positionCount);
	positions->emplace_back(duplicatePositionExternalID);
	positions->emplace_back(23423535l);
	positions->emplace_back(duplicatePositionExternalID);

	for (int i = 0; i < positionCount; i++) {
		EXPECT_CALL(positionsPtr->at(i), getReferenceValue())
			.WillOnce(testing::Return(0.1));
	}

	const std::string expExceptionString = "Duplicate Portfolio Position ID";

	try {
		sic::Portfolio<sic::MockPosition> duplicatePositionPortfolio(
			std::move(positions), expExternalID);
		FAIL() << "Able to create Portfolio with duplicate Position.";
	} catch (const std::invalid_argument &e) {
		ASSERT_EQ(expExceptionString, e.what());
	} catch (...) {
		FAIL() << "Unexpected  error."; // Unexpected exception type.
	}
}

} // namespace
