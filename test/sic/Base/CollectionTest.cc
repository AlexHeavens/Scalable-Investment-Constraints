#include <gtest/gtest.h>

#include "sic/Base/Collection.hh"

#include <vector>

namespace {

class CollectionTest : public testing::Test {
public:
	static constexpr int expIntCount = 10;
	std::vector<int> intVector;

	void SetUp() override {

		intVector.reserve(expIntCount);
		for (int i = 0; i < expIntCount; i++) {
			intVector.push_back(i);
		}
	}
};
constexpr int CollectionTest::expIntCount;

TEST_F(CollectionTest, Construct) {

	sic::Iterators<int> expIterators(intVector);

	sic::Collection<int, std::vector<int>> intCollection(intVector);

	ASSERT_EQ(intCollection.size(), intVector.size());
}

} // namespace
