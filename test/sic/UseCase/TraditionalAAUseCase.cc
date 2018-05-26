#include <gtest/gtest.h>

#include "sic/Model/RegularFilterTreeFactory.hh"

namespace {

class TraditionalAAUseCase : public testing::Test {};

TEST_F(TraditionalAAUseCase, Execute) {

	constexpr sic::External::ID filterTreeID = 12345;
	constexpr unsigned treeDepth = 3;
	constexpr unsigned nodeDegree = 4;
	sic::RegularFilterTreeFactory filterTreeFactory(treeDepth, nodeDegree);

	std::unique_ptr<sic::AbstractFilterTree> filterTree =
		filterTreeFactory.create(filterTreeID);
}

} // namespace
