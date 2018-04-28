#include <gtest/gtest.h>
#include <random>

#include "sic/Portfolio/Asset.hh"

namespace {

class AssetTest : public testing::Test {
public:
	void SetUp() override {}
};

TEST_F(AssetTest, CreateValid) {

	const sic::Price price = 100.00;

	// Random selection of Asset classes.
	const unsigned seed = 39487;
	std::uniform_int_distribution<unsigned> distr(0, 500000);
	std::mt19937 randomGen(seed);
	const int expClassCount = 100;
	sic::Asset::ClassVector expClasses;
	expClasses.reserve(expClassCount);
	for (int i = 0; i < expClassCount; i++) {
		expClasses.push_back(distr(randomGen));
	}
	std::experimental::optional<const sic::Asset::ClassVector *> optExpClasses(
		&expClasses);

	sic::Asset validAsset(price, optExpClasses);

	ASSERT_EQ(price, validAsset.getReferencePrice());

	// Test asset class iteration.
	std::pair<sic::Asset::ClassIterator, sic::Asset::ClassIterator>
		classIterator = validAsset.getClassIterators();

	int expClassIndex = 0;
	while (classIterator.first != classIterator.second) {

		ASSERT_TRUE(expClassIndex < expClassCount);
		ASSERT_EQ(expClasses.at(expClassIndex), *(classIterator.first));
		classIterator.first++;
		expClassIndex++;
	}

	ASSERT_EQ(expClassCount, expClassIndex);
}

} // namespace
