#include <gtest/gtest.h>

#include <unordered_set>

#include "sic/Base/Algorithm/ReservoirSampling.hh"

namespace {

class ReservoirSamplingTest : public testing::Test {};

TEST_F(ReservoirSamplingTest, FilterValid) {

	constexpr int vectorSize = 50;
	std::vector<int> intVector;
	std::unordered_set<const int *> expPossibleSamples;

	intVector.reserve(vectorSize);
	for (int i = 0; i < vectorSize; i++) {
		intVector.push_back(i);
		expPossibleSamples.insert(&intVector.at(i));
	}

	sic::Iterators<int> validIterators(intVector);

	const size_t expSampleCount = 10;

	auto samples = sic::resevoirSample(validIterators, expSampleCount);

	ASSERT_EQ(samples->size(), expSampleCount);

	std::unordered_set<const int *> intsSampled;
	for (auto sample : *samples) {

		ASSERT_TRUE(intsSampled.find(sample) == intsSampled.end())
			<< "Int sampled twice.";
		intsSampled.insert(sample);

		auto validSample =
			expPossibleSamples.find(sample) != expPossibleSamples.end();
		ASSERT_TRUE(validSample) << "Sample found outside input vector.";
	}
}

} // namespace
