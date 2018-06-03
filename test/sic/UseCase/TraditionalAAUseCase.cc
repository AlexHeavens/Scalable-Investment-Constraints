#include <gtest/gtest.h>

#include <random>

#include "sic/EvaluationContext.hh"
#include "sic/Model/RegularFilterTreeFactory.hh"

namespace {

class TraditionalAAUseCase : public testing::Test {};

TEST_F(TraditionalAAUseCase, Execute) {

	constexpr int randomSeed = 34534;
	std::mt19937 randomGen(randomSeed);

	sic::EvaluationContext context;

	// Generate FilterTrees.
	sic::External::ID nextFilterTreeID = 1000;
	struct FilterTreeParameters {
		unsigned treeCount, treeDepth, nodeDegree;
		FilterTreeParameters(unsigned treeCount, unsigned treeDepth,
							 unsigned nodeDegree)
			: treeCount(treeCount), treeDepth(treeDepth),
			  nodeDegree(nodeDegree) {}
	};

	// Typical cross-bank number of FilterTrees.
	std::vector<FilterTreeParameters> filterTreeParams;
	filterTreeParams.emplace_back(5, 3, 4);
	filterTreeParams.emplace_back(5, 3, 3);
	filterTreeParams.emplace_back(5, 2, 5);
	filterTreeParams.emplace_back(5, 2, 3);

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

	// Generate Assets.

	// 100 class groups, up to 4 classes per group.
	constexpr unsigned classGroupCount = 100;
	constexpr unsigned groupJump = 100;
	constexpr unsigned classesPerGroup = 4;

	// Give each Asset a random class from each group.
	std::uniform_int_distribution<> classDistribution(0, classesPerGroup - 1);

	constexpr sic::External::ID assetIDsFrom = 1000;
	constexpr unsigned assetCount = 100000;
	for (sic::Asset::ID assetID = assetIDsFrom;
		 assetID < assetIDsFrom + assetCount; assetID++) {

		std::unique_ptr<sic::AbstractAsset::ClassSet> assetClasses(
			new sic::AbstractAsset::ClassSet);
		for (unsigned classGroup = 0; classGroup < classGroupCount;
			 classGroup++) {
			const sic::Asset::Class groupClass =
				classGroup * groupJump + classDistribution(randomGen);
			assetClasses->insert(groupClass);
		}

		auto asset =
			std::make_unique<sic::Asset>(assetID, std::move(assetClasses));
		context.getAssetCache().add(std::move(asset));
	}
}

} // namespace
