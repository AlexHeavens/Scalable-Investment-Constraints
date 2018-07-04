#include <gtest/gtest.h>

#include <chrono>
#include <iostream>
#include <random>
#include <utility>

#include "sic/AAPortfolioFactory.hh"
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
	static sic::EvaluationContext context;
	static bool setup;

	static void
	generateFilterTrees(sic::EvaluationContext *const evaluationContext) {

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

				evaluationContext->getFilterTreeCache().add(
					std::make_unique<sic::FilterTree>(nextFilterTreeID));
				sic::AbstractFilterTree &filterTree =
					evaluationContext->getFilterTreeCache().get(
						nextFilterTreeID);

				filterTreeFactory.create(filterTree);
				nextFilterTreeID++;
			}
		}
	}

	static void generateAssets(sic::EvaluationContext *const evaluationContext,
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
			evaluationContext->getAssetCache().add(std::move(asset));
		}
	}

	static void
	generateAssetAllocations(sic::EvaluationContext *evaluationContext) {

		const sic::Source<std::unique_ptr<sic::AbstractFilterTree>>
			&filterTreeSource = evaluationContext->getFilterTreeCache();
		const auto &assetSource = evaluationContext->getAssetCache();
		auto &aaSource = evaluationContext->getAssetAllocationCache();
		auto &mpfSource = evaluationContext->getModelPortfolioCache();

		constexpr int mpfAssetCount = 5;

		sic::RegularAAFactory factory(filterTreeSource, assetSource,
									  mpfAssetCount);

		constexpr int aasPerFilterTree = 10;
		const int filterTreeCount = filterTreeSource.size();
		const int aaCount = filterTreeCount * aasPerFilterTree;

		constexpr int logEvery = 10;
		for (int i = 0; i < aaCount; i++) {

			if (i % logEvery == 0) {
				std::cout << "Generating AA " << i << "\n";
			}

			auto newAAMPFs = factory.create();
			std::unique_ptr<sic::AbstractAssetAllocation> &newAA =
				newAAMPFs.first;
			const auto &newMPFs = newAAMPFs.second;

			aaSource.add(std::move(newAA));

			for (auto &newMPF : *newMPFs) {
				mpfSource.add(std::move(newMPF));
			}
		}
	}

	static void generatePortfolios(sic::EvaluationContext *evaluationContext) {

		const auto &aaSource = evaluationContext->getAssetAllocationCache();
		auto &portfolioSource = evaluationContext->getPortfolioCache();

		constexpr int portfoliosPerAA = 1000;
		constexpr sic::Value portfolioValue = 1000000.0;

		constexpr int logEvery = 50000;
		int portfolioCount = 0;

		for (const auto &aa : aaSource) {

			sic::AAPortfolioFactory factory(*aa, portfolioValue,
											portfolioCount);

			for (int i = 0; i < portfoliosPerAA; i++) {

				if (portfolioCount % logEvery == 0) {
					std::cout << "Generating Portfolio " << portfolioCount
							  << "\n";
				}

				auto newPortfolio = factory.create();
				portfolioSource.add(std::move(newPortfolio));

				portfolioCount++;
			}
		}
	}

	void SetUp() override {

		if (setup) {
			return;
		}

		randomGen = std::mt19937(randomSeed);
		generateFilterTrees(&context);
		generateAssets(&context, &randomGen);
		generateAssetAllocations(&context);
		generatePortfolios(&context);

		setup = true;
	}
};

constexpr int TraditionalAA::randomSeed;
sic::EvaluationContext TraditionalAA::context;
bool TraditionalAA::setup = false;

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
	std::cout << "TraditionalAAUseCase::FilterAsset Wall Time (ms): "
			  << durationMilliseconds.count() << std::endl;
}

TEST_F(TraditionalAA, GenerateRestrictionResults) {

	auto startTime = std::chrono::high_resolution_clock::now();

	for (const auto &portfolio : context.getPortfolioCache()) {
		for (const auto &aa : portfolio->getAssetAllocations()) {
			auto results = aa->generateRestrictionResults(*portfolio);
		}
	}

	auto finishTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> durationMilliseconds =
		finishTime - startTime;
	std::cout
		<< "TraditionalAAUseCase::GenerateRestrictionResults Wall Time (ms): "
		<< durationMilliseconds.count() << std::endl;
}

} // namespace
