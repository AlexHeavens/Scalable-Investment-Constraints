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

	constexpr sic::External::ID externalAID = 999;
	constexpr sic::External::ID externalBID = 1;
	constexpr sic::External::ID externalCID = 435436;
	auto *externalA = new MockExternal(externalAID);
	auto *externalA2 = new MockExternal(externalAID);
	auto *externalB = new MockExternal(externalBID);
	auto *externalC = new MockExternal(externalCID);

	std::unique_ptr<MockExternal> externalAPtr(externalA);
	std::unique_ptr<MockExternal> externalA2Ptr(externalA2);
	std::unique_ptr<MockExternal> externalBPtr(externalB);
	std::unique_ptr<MockExternal> externalCPtr(externalC);

	sic::ExternalCache<MockExternal> externalCache;
	ASSERT_FALSE(externalCache.contains(externalAID));
	ASSERT_FALSE(externalCache.contains(externalBID));
	ASSERT_FALSE(externalCache.contains(externalCID));

	// Add to cache via move.
	externalCache.add(std::move(externalAPtr));
	ASSERT_TRUE(externalCache.contains(externalAID));
	const MockExternal &retrievedExternalA1 = externalCache.get(externalAID);
	ASSERT_EQ(retrievedExternalA1.getExternalID(), externalA->getExternalID());
	ASSERT_EQ(&retrievedExternalA1, externalA);

	// Second add does not affect the cache.
	externalCache.add(std::move(externalA2Ptr));
	ASSERT_TRUE(externalCache.contains(externalAID));
	const MockExternal &retrievedExternalA2 = externalCache.get(externalAID);
	ASSERT_EQ(&retrievedExternalA1, &retrievedExternalA2);
	ASSERT_EQ(retrievedExternalA2.getExternalID(), externalA->getExternalID());
	ASSERT_EQ(&retrievedExternalA2, externalA);

	externalCache.add(std::move(externalBPtr));
	externalCache.add(std::move(externalCPtr));

	ASSERT_TRUE(externalCache.contains(externalAID));
	ASSERT_TRUE(externalCache.contains(externalBID));
	ASSERT_TRUE(externalCache.contains(externalCID));

	// Sanity-check iterators.
	auto items = externalCache.getItems();
	ASSERT_TRUE(items.current() != items.end())
		<< "Cache iterators matches the end of the item set.";
}

} // namespace
