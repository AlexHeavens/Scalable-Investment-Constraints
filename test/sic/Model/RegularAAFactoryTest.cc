#include <gtest/gtest.h>

#include <memory>

#include "sic/Model/MockFilterTree.hh"
#include "sic/Model/RegularAAFactory.hh"

namespace {

class RegularAAFactoryTest : public testing::Test {

public:
	class MockFilterTreeSource
		: public sic::Source<std::unique_ptr<sic::AbstractFilterTree>> {

	public:
		MOCK_METHOD0(
			getItems,
			sic::Iterators<std::unique_ptr<sic::AbstractFilterTree>>());
	};
};

TEST_F(RegularAAFactoryTest, CreateValid) {

	constexpr int filterTreeCount = 3;
	std::vector<std::unique_ptr<sic::AbstractFilterTree>> filterTrees;
	for (int i = 0; i < filterTreeCount; i++) {
		auto treePtr = std::make_unique<sic::MockFilterTree>();
		filterTrees.push_back(std::move(treePtr));
	}

	sic::Iterators<std::unique_ptr<sic::AbstractFilterTree>>::It begin(
		filterTrees.begin());
	sic::Iterators<std::unique_ptr<sic::AbstractFilterTree>>::It end(
		filterTrees.end());
	sic::Iterators<std::unique_ptr<sic::AbstractFilterTree>> filterTreesIt(
		begin, end);
	MockFilterTreeSource filterTreeSource;
	EXPECT_CALL(filterTreeSource, getItems())
		.WillOnce(testing::Return(filterTreesIt))
		.WillOnce(testing::Return(filterTreesIt));

	sic::RegularAAFactory factory(filterTreeSource);

	constexpr int expAACount = 6;
	for (int i = 0; i < expAACount; i++) {

		auto newAA = factory.create();

		// FilterTrees are expected to be used in a round-robin fashion.
		const int expAAIndex = i % filterTreeCount;
		const auto &expAA = *filterTrees.at(expAAIndex);
		ASSERT_EQ(&newAA->getFilterTree(), &expAA)
			<< "AA created with unexpected FilterTree.";
	}
}

} // namespace
