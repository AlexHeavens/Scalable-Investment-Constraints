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

} // namespace
