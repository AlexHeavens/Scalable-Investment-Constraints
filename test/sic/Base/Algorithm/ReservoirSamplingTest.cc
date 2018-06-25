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

	sic::Iterators<int> population(intVector);

	const size_t expSampleCount = 10;

	auto samples = sic::resevoirSample(population, expSampleCount);

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

TEST_F(ReservoirSamplingTest, PopulationSmallerThanSampleSize) {

	constexpr int vectorSize = 5;
	std::vector<int> intVector;

	intVector.reserve(vectorSize);
	for (int i = 0; i < vectorSize; i++) {
		intVector.push_back(i);
	}

	sic::Iterators<int> population(intVector);
	constexpr size_t expSampleCount = 6;
	const std::string expError = "Population size (" +
								 std::to_string(vectorSize) +
								 ") is smaller than requested sample count (" +
								 std::to_string(expSampleCount) + ").";

	try {
		auto _ = sic::resevoirSample(population, expSampleCount);
		FAIL() << "Able to sample where population is smaller than expected "
				  "sample count.";
	} catch (const std::invalid_argument &e) {
		ASSERT_EQ(e.what(), expError)
			<< "Error message does not match expected.";
	} catch (const std::exception &e) {
		FAIL() << "Unexpected exception: " << e.what() << ".";
	}
}

} // namespace
