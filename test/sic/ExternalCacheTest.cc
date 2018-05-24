#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "sic/ExternalCache.hh"
#include "sic/Portfolio/Asset.hh"

namespace {

class ExternalCacheTest : public testing::Test {

public:
	class MockAsset : public sic::AbstractAsset {

	public:
		explicit MockAsset(sic::External::ID externalID)
			: sic::AbstractAsset(externalID) {}
	};
};

TEST_F(ExternalCacheTest, CreateValid) {

	const sic::External::ID assetAID = 999;
	const sic::External::ID assetBID = 1;
	const sic::External::ID assetCID = 435436;
	const MockAsset assetA(assetAID);
	const MockAsset assetB(assetBID);
	const MockAsset assetC(assetCID);

	sic::ExternalCache<MockAsset> assetCache;
	ASSERT_FALSE(assetCache.contains(assetAID));
	ASSERT_FALSE(assetCache.contains(assetBID));
	ASSERT_FALSE(assetCache.contains(assetCID));

	// Add to cache via copy constructor.
	assetCache.add(assetA);
	ASSERT_TRUE(assetCache.contains(assetAID));
	const sic::AbstractAsset &retrievedAssetA1 = assetCache.get(assetAID);
	ASSERT_EQ(retrievedAssetA1.getExternalID(), assetA.getExternalID());
	ASSERT_FALSE(&retrievedAssetA1 == &assetA);

	// Second add does not affect the cache.
	assetCache.add(assetA);
	ASSERT_TRUE(assetCache.contains(assetAID));
	const sic::AbstractAsset &retrievedAssetA2 = assetCache.get(assetAID);
	ASSERT_EQ(&retrievedAssetA1, &retrievedAssetA2);
	ASSERT_EQ(retrievedAssetA2.getExternalID(), assetA.getExternalID());
	ASSERT_FALSE(&retrievedAssetA2 == &assetA);

	assetCache.add(assetB);
	assetCache.add(assetC);

	ASSERT_TRUE(assetCache.contains(assetAID));
	ASSERT_TRUE(assetCache.contains(assetBID));
	ASSERT_TRUE(assetCache.contains(assetCID));
}

} // namespace
