#include <gtest/gtest.h>
#include <random>

#include "sic/Portfolio/Asset.hh"

namespace {

class AssetTest : public testing::Test {};

TEST_F(AssetTest, CreateValidAsset) {

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
	sic::Asset validAsset(expExternalID, std::move(optExpClasses));

	ASSERT_EQ(expExternalID, validAsset.getExternalID());

	// Test asset class iteration.
	auto classIterator = validAsset.getClassIterators();

	int expClassIndex = 0;
	while (classIterator.current() != classIterator.end()) {

		ASSERT_TRUE(expClassIndex < expClassCount);
		ASSERT_TRUE(validAsset.hasClass(*classIterator.current()));

		classIterator.current()++;
		expClassIndex++;
	}

	// Known random seed, this class is not added.
	const sic::Asset::Class nonAddedClass = 54367;
	ASSERT_FALSE(validAsset.hasClass(nonAddedClass));

	ASSERT_EQ(expClassCount, expClassIndex);
}

TEST_F(AssetTest, EmptyAssetClassList) {

	const sic::External::ID expExternalID = 43543653l;

	// Create Asset, default empty class list.
	sic::Asset assetA(expExternalID);

	ASSERT_FALSE(assetA.hasClass(3534));

	auto assetClassItsA = assetA.getClassIterators();
	ASSERT_EQ(assetClassItsA.current(), assetClassItsA.end());

	// Create Asset, explicitly empty class list.
	auto expClassSet = std::make_unique<sic::Asset::ClassSet>();
	sic::Asset assetB(expExternalID, std::move(expClassSet));

	ASSERT_FALSE(assetA.hasClass(3534));

	auto assetClassItsB = assetB.getClassIterators();
	ASSERT_EQ(assetClassItsB.current(), assetClassItsB.end());
}

} // namespace
