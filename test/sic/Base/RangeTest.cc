#include <gtest/gtest.h>

#include "sic/Base/Range.hh"

#include <limits>

namespace {

class RangeTest : public testing::Test {};

TEST_F(RangeTest, CreateValid) {

	constexpr float expTarget = 1.0;
	constexpr float expMin = expTarget - std::numeric_limits<float>::epsilon();
	constexpr float expMax = expTarget + std::numeric_limits<float>::epsilon();

	sic::Range validRange(expMin, expTarget, expMax);

	ASSERT_EQ(validRange.min, expMin);
	ASSERT_EQ(validRange.target, expTarget);
	ASSERT_EQ(validRange.max, expMax);
}

TEST_F(RangeTest, CreateInvalidMin) {

	constexpr float expTarget = 1.0;
	constexpr float expMin = expTarget + std::numeric_limits<float>::epsilon();
	constexpr float expMax = 1.0;
	const std::string expError = "Range min must be less or equal to target.";

	try {
		sic::Range validRange(expMin, expTarget, expMax);
		FAIL() << "Able to create Range with min > target.";
	} catch (std::invalid_argument &e) {
		ASSERT_EQ(e.what(), expError);
	}
}

TEST_F(RangeTest, CreateInvalidMax) {

	constexpr float expTarget = 1.0;
	constexpr float expMax = expTarget - std::numeric_limits<float>::epsilon();
	constexpr float expMin = 1.0;
	const std::string expError =
		"Range max must be greater or equal to target.";

	try {
		sic::Range validRange(expMin, expTarget, expMax);
		FAIL() << "Able to create Range with max < target.";
	} catch (std::invalid_argument &e) {
		ASSERT_EQ(e.what(), expError);
	}
}

TEST_F(RangeTest, Equality) {

	const sic::Range<float> range1(0.1, 0.2, 0.3);
	const sic::Range<float> range2(0.1, 0.2, 0.3);

	const sic::Range<float> range3(0.2, 0.2, 0.3);
	const sic::Range<float> range4(0.1, 0.3, 0.3);
	const sic::Range<float> range5(0.1, 0.2, 0.2);

	ASSERT_TRUE(range1 == range2);
	ASSERT_FALSE(range1 == range3);
	ASSERT_FALSE(range1 == range4);
}

} // namespace
