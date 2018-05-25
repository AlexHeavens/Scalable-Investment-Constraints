#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "sic/ExternalCache.hh"

namespace {

class ExternalCacheTest : public testing::Test {

public:
	class MockExternal : public sic::External {

	public:
		explicit MockExternal(sic::External::ID externalID)
			: sic::External(externalID) {}
	};
};

TEST_F(ExternalCacheTest, CreateValid) {

	const sic::External::ID externalAID = 999;
	const sic::External::ID externalBID = 1;
	const sic::External::ID externalCID = 435436;
	const MockExternal externalA(externalAID);
	const MockExternal externalB(externalBID);
	const MockExternal externalC(externalCID);

	sic::ExternalCache<MockExternal> externalCache;
	ASSERT_FALSE(externalCache.contains(externalAID));
	ASSERT_FALSE(externalCache.contains(externalBID));
	ASSERT_FALSE(externalCache.contains(externalCID));

	// Add to cache via copy constructor.
	externalCache.add(externalA);
	ASSERT_TRUE(externalCache.contains(externalAID));
	const MockExternal &retrievedExternalA1 = externalCache.get(externalAID);
	ASSERT_EQ(retrievedExternalA1.getExternalID(), externalA.getExternalID());
	ASSERT_FALSE(&retrievedExternalA1 == &externalA);

	// Second add does not affect the cache.
	externalCache.add(externalA);
	ASSERT_TRUE(externalCache.contains(externalAID));
	const MockExternal &retrievedExternalA2 = externalCache.get(externalAID);
	ASSERT_EQ(&retrievedExternalA1, &retrievedExternalA2);
	ASSERT_EQ(retrievedExternalA2.getExternalID(), externalA.getExternalID());
	ASSERT_FALSE(&retrievedExternalA2 == &externalA);

	externalCache.add(externalB);
	externalCache.add(externalC);

	ASSERT_TRUE(externalCache.contains(externalAID));
	ASSERT_TRUE(externalCache.contains(externalBID));
	ASSERT_TRUE(externalCache.contains(externalCID));
}

} // namespace
