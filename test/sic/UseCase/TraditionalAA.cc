#include <gtest/gtest.h>

#include <chrono>
#include <iostream>
#include <random>

#include "sic/EvaluationContext.hh"
#include "sic/Model/FilterTree.hh"
#include "sic/Model/RegularAAFactory.hh"
#include "sic/Model/RegularFilterTreeFactory.hh"
#include "sic/Portfolio/Asset.hh"

namespace {

class TraditionalAA : public testing::Test {

public:
	static constexpr int randomSeed = 34534;
	std::mt19937 randomGen;
	sic::EvaluationContext context;

	static void generateFilterTrees(sic::EvaluationContext *const context) {

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
		filterTreeParams.emplace_back(5, 3, 5);
		filterTreeParams.emplace_back(5, 3, 4);
		filterTreeParams.emplace_back(5, 2, 5);
		filterTreeParams.emplace_back(5, 2, 4);

		for (auto &filterTreeParam : filterTreeParams) {
			sic::RegularFilterTreeFactory filterTreeFactory(
				filterTreeParam.treeDepth, filterTreeParam.nodeDegree);

			for (unsigned i = 0; i < filterTreeParam.treeCount; i++) {

				context->getFilterTreeCache().add(
					std::make_unique<sic::FilterTree>(nextFilterTreeID));
				sic::AbstractFilterTree &filterTree =
					context->getFilterTreeCache().get(nextFilterTreeID);

				filterTreeFactory.create(filterTree);
				nextFilterTreeID++;
			}
		}
	}

	static void generateAssets(sic::EvaluationContext *const context,
							   std::mt19937 *const randomGen) {

		// 100 class groups, up to 4 classes per group.
		constexpr unsigned classGroupCount = 100;
		constexpr unsigned groupJump = 100;
		constexpr unsigned classesPerGroup = 4;

		// Give each Asset a random class from each group.
		std::uniform_int_distribution<> classDistribution(0,
														  classesPerGroup - 1);

		constexpr sic::External::ID assetIDsFrom = 1000;
		constexpr unsigned assetCount = 100000;
		for (sic::Asset::ID assetID = assetIDsFrom;
			 assetID < assetIDsFrom + assetCount; assetID++) {

			std::unique_ptr<sic::AbstractAsset::ClassSet> assetClasses(
				new sic::AbstractAsset::ClassSet);
			for (unsigned classGroup = 0; classGroup < classGroupCount;
				 classGroup++) {
				const sic::Asset::Class groupClass =
					classGroup * groupJump + classDistribution(*randomGen);
				assetClasses->insert(groupClass);
			}

			auto asset =
				std::make_unique<sic::Asset>(assetID, std::move(assetClasses));
			context->getAssetCache().add(std::move(asset));
		}
	}

	void SetUp() override {
		randomGen = std::mt19937(randomSeed);
		generateFilterTrees(&context);
		generateAssets(&context, &randomGen);
	}
};

constexpr int TraditionalAA::randomSeed;

TEST_F(TraditionalAA, FilterAsset) {

	auto startTime = std::chrono::high_resolution_clock::now();

	auto filterTrees = context.getFilterTreeCache().getItems();

	while (filterTrees.remaining()) {

		auto &filterTree = **filterTrees.current();

		auto assets = context.getAssetCache().getItems();

		while (assets.remaining()) {

			auto &asset = **assets.current();

			auto &leaf = filterTree.getLeafNode(asset);
			unused(leaf);

			assets.current()++;
		}

		filterTrees.current()++;
	}

	auto finishTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> durationMilliseconds =
		finishTime - startTime;
	std::cout << "TraditionalAAUseCase Wall Time (ms): "
			  << durationMilliseconds.count() << std::endl;
}

} // namespace
