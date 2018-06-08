#include <gtest/gtest.h>

#include "sic/Base/Iterators.hh"

#include <vector>

namespace {

class IteratorsTest : public testing::Test {};

TEST_F(IteratorsTest, CreateValidFull) {

	constexpr int expIntCount = 10;
	std::vector<int> intVector;
	intVector.reserve(expIntCount);
	for (int i = 0; i < expIntCount; i++) {
		intVector.push_back(i);
	}

	sic::Iterators validIterators(intVector.begin(), intVector.end());

	int intCount = 0;
	auto vectorIt = intVector.begin();

	while (validIterators.remaining()) {

		ASSERT_EQ(validIterators.current(), vectorIt)
			<< "Iterator does not match.";
		ASSERT_EQ(*validIterators.current(), *vectorIt)
			<< "Value at iterator does not match.";

		intCount++;
		vectorIt++;

		validIterators.current()++;
	}

	ASSERT_EQ(intCount, expIntCount)
		<< "Unexpected number of items iterated through.";
}

TEST_F(IteratorsTest, CreateValidEmpty) {

	std::vector<int> intVector;

	sic::Iterators validIterators(intVector.begin(), intVector.end());

	ASSERT_EQ(validIterators.current(), intVector.begin());
	ASSERT_EQ(validIterators.end(), intVector.end());
	ASSERT_EQ(validIterators.current(), validIterators.end());
	ASSERT_FALSE(validIterators.remaining());
}

} // namespace
