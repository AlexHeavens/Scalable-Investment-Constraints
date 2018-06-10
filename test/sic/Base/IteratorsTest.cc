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

	sic::Iterators<int>::It begin(intVector.begin());
	const sic::Iterators<int>::It end(intVector.end());

	sic::Iterators<int> validIterators(begin, end);

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

	sic::Iterators<int>::It begin(intVector.begin());
	const sic::Iterators<int>::It end(intVector.end());

	sic::Iterators<int> validIterators(begin, end);

	ASSERT_EQ(validIterators.current(), validIterators.end());
	ASSERT_FALSE(validIterators.remaining());
}

} // namespace
