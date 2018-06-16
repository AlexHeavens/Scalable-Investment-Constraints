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

	sic::Iterators<int> validIterators(intVector);

	int intCount = 0;
	auto vectorIt = intVector.begin();

	while (validIterators.remaining()) {

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

	sic::Iterators<int> validIterators(intVector);

	ASSERT_EQ(validIterators.current(), validIterators.end());
	ASSERT_FALSE(validIterators.remaining());
}

TEST_F(IteratorsTest, CreateValidPointerBased) {

	constexpr int expIntCount = 10;
	auto intVector = std::make_unique<std::vector<int>>();

	intVector->reserve(expIntCount);
	for (int i = 0; i < expIntCount; i++) {
		intVector->push_back(i);
	}

	sic::Iterators<int> intIterators(intVector);
	int intCount = 0;
	auto vectorIt = intVector->begin();

	while (intIterators.remaining()) {

		ASSERT_EQ(*intIterators.current(), *vectorIt)
			<< "Value at iterator does not match.";

		intCount++;
		vectorIt++;

		intIterators.current()++;
	}

	ASSERT_EQ(intCount, expIntCount)
		<< "Unexpected number of items iterated through.";
}

} // namespace
