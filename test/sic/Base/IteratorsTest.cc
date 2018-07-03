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

TEST_F(IteratorsTest, ComparisonOperator) {

	constexpr int expIntCount1 = 10;
	std::vector<int> intVector1, intVector2;

	intVector1.reserve(expIntCount1);
	intVector2.reserve(expIntCount1);
	for (int i = 0; i < expIntCount1; i++) {
		intVector1.push_back(i);
		intVector2.push_back(i);
	}

	constexpr int expIntCount2 = 5;
	std::vector<int> intVector3;

	intVector3.reserve(expIntCount2);
	for (int i = 0; i < expIntCount2; i++) {
		intVector3.push_back(i);
	}

	sic::Iterators<int> intIterators1(intVector1);
	sic::Iterators<int> intIterators2(intVector1);

	sic::Iterators<int> intIterators3(intVector1);
	intIterators3.current()++;
	sic::Iterators<int> intIterators4(intVector3);
	sic::Iterators<int> intIterators5(intVector2);

	ASSERT_TRUE(intIterators1 == intIterators1);
	ASSERT_TRUE(intIterators1 == intIterators2);

	ASSERT_FALSE(intIterators1 == intIterators3);
	ASSERT_FALSE(intIterators1 == intIterators4);
	ASSERT_FALSE(intIterators1 == intIterators5);
}

} // namespace
