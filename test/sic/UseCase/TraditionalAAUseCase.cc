#include <gtest/gtest.h>

#include "sic/EvaluationContext.hh"
#include "sic/Model/RegularFilterTreeFactory.hh"

namespace {

class TraditionalAAUseCase : public testing::Test {};

TEST_F(TraditionalAAUseCase, Execute) {

	sic::EvaluationContext context;

	sic::External::ID nextFilterTreeID = 1000;
	struct FilterTreeParameters {
		unsigned treeCount, treeDepth, nodeDegree;
		FilterTreeParameters(unsigned treeCount, unsigned treeDepth,
							 unsigned nodeDegree)
			: treeCount(treeCount), treeDepth(treeDepth),
			  nodeDegree(nodeDegree) {}
	};

	std::vector<FilterTreeParameters> filterTreeParams;
	filterTreeParams.emplace_back(5, 3, 4);
	filterTreeParams.emplace_back(5, 3, 3);
	filterTreeParams.emplace_back(5, 2, 10);
	filterTreeParams.emplace_back(5, 2, 5);

	for (auto &filterTreeParam : filterTreeParams) {

		sic::RegularFilterTreeFactory filterTreeFactory(
			filterTreeParam.treeDepth, filterTreeParam.nodeDegree);

		for (unsigned i = 0; i < filterTreeParam.treeCount; i++) {

			context.getFilterTreeCache().add(
				std::make_unique<sic::FilterTree>(nextFilterTreeID));
			auto &filterTree =
				context.getFilterTreeCache().get(nextFilterTreeID);

			filterTreeFactory.create(filterTree);
			nextFilterTreeID++;
		}
	}
}

} // namespace
