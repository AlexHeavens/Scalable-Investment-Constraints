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
	std::vector<sic::MockPosition *> expPositionAddresses;

	// Store the addresses of the mock positions to compare after
	// intiialisation.  Warning: must be careful to reserve vector space to
	// avoid re-addressing due to dynamic vector expansion.
	expPositions->reserve(expPositionCount);
	expPositionAddresses.reserve(expPositionCount);

	sic::External::ID nextPositionID = 123;
	for (int i = 0; i < expPositionCount; i++) {
		expPositions->emplace_back(nextPositionID++);
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
	sic::MockPosition positionA(duplicatePositionExternalID);
	sic::MockPosition positionB(23423535l);
	sic::MockPosition positionC(duplicatePositionExternalID);

	auto inputPositionVector =
		std::make_unique<std::vector<sic::MockPosition>>();
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
