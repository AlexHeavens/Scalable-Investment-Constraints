#include <gtest/gtest.h>
#include <random>

#include "sic/Portfolio/Asset.hh"

namespace {

class AssetTest : public testing::Test {
public:
	void SetUp() override {}
};

TEST_F(AssetTest, CreateValidAsset) {

	const sic::Price price = 100.00;
	const sic::External::ID expExternalID = 43543653l;

	// Random selection of Asset classes.
	const unsigned seed = 39487;
	std::uniform_int_distribution<unsigned> distr(0, 500000);
	std::mt19937 randomGen(seed);
	const int expClassCount = 100;

	// Store reference to underlying ClassSet for assertion comparison as
	// address as moving will empty the unique_ptr.
	std::unique_ptr<sic::Asset::ClassSet> expClasses(new sic::Asset::ClassSet);

	expClasses->reserve(expClassCount);
	for (int i = 0; i < expClassCount; i++) {
		expClasses->emplace(distr(randomGen));
	}
	std::experimental::optional<std::unique_ptr<sic::Asset::ClassSet>>
		optExpClasses(std::move(expClasses));

	// Create Asset
	sic::Asset validAsset(price, expExternalID, std::move(optExpClasses));

	ASSERT_EQ(price, validAsset.getReferencePrice());
	ASSERT_EQ(expExternalID, validAsset.getExternalID());

	// Test asset class iteration.
	std::pair<sic::Asset::ClassIterator, sic::Asset::ClassIterator>
		classIterator = validAsset.getClassIterators();

	int expClassIndex = 0;
	while (classIterator.first != classIterator.second) {

		ASSERT_TRUE(expClassIndex < expClassCount);
		classIterator.first++;
		expClassIndex++;
	}

	ASSERT_EQ(expClassCount, expClassIndex);
}

} // namespace
