#ifndef SIC_TRADITIONALAACONTEXT_H_
#define SIC_TRADITIONALAACONTEXT_H_

#include <iostream>
#include <random>
#include <vector>

#include "sic/AAPortfolioFactory.hh"
#include "sic/EvaluationContext.hh"
#include "sic/Model/FilterTree.hh"
#include "sic/Model/RegularAAFactory.hh"
#include "sic/Model/RegularFilterTreeFactory.hh"
#include "sic/Portfolio/Asset.hh"

namespace sic {

class TraditionalAAContext {

private:
	static constexpr int randomSeed = 34534;
	std::mt19937 randomGen;
	sic::EvaluationContext context;
	std::vector<std::unique_ptr<sic::AbstractAsset>> nonModelAssets;
	static constexpr int nonModelAssetCount = 20;

	static std::unique_ptr<sic::TraditionalAAContext> singleton;

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

	static void generateAssets(
		sic::EvaluationContext *const evaluationContext,
		std::mt19937 *const randomGen,
		std::vector<std::unique_ptr<sic::AbstractAsset>> *nonModelAssets) {

		// 100 class groups, up to 4 classes per group.
		constexpr unsigned classGroupCount = 100;
		constexpr unsigned groupJump = 100;
		constexpr unsigned classesPerGroup = 4;

		// Give each Asset a random class from each group.
		std::uniform_int_distribution<> classDistribution(0,
														  classesPerGroup - 1);

		constexpr sic::External::ID assetIDsFrom = 1000;
		constexpr unsigned assetCount = 100000;
		sic::Asset::ID assetID = 0;
		for (assetID = assetIDsFrom; assetID < assetIDsFrom + assetCount;
			 assetID++) {

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

		// Generate Assets guaranteed to be in no model.
		for (int i = 0; i < nonModelAssetCount; i++) {
			nonModelAssets->emplace_back(new sic::Asset(assetID++));
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

	static void generatePortfolios(
		const std::vector<std::unique_ptr<sic::AbstractAsset>> &nonModelAssets,
		sic::EvaluationContext *evaluationContext) {

		const auto &aaSource = evaluationContext->getAssetAllocationCache();
		auto &portfolioSource = evaluationContext->getPortfolioCache();

		constexpr int portfoliosPerAA = 5300;
		constexpr sic::Value portfolioValue = 1000000.0;

		constexpr int logEvery = 50000;
		int portfolioCount = 0;

		for (const auto &aa : aaSource) {

			const float validPortfolioRate = 0.9;
			const float validPositionRate = 0.9;

			sic::AAPortfolioFactory factory(*aa, portfolioValue, portfolioCount,
											0, validPortfolioRate,
											validPositionRate, nonModelAssets);

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

public:
	static sic::TraditionalAAContext &getSingleton() {
		if (!singleton) {
			singleton.reset(new sic::TraditionalAAContext());
		}
		return *singleton;
	}

	TraditionalAAContext() {

		randomGen = std::mt19937(randomSeed);
		generateFilterTrees(&context);
		generateAssets(&context, &randomGen, &nonModelAssets);
		generateAssetAllocations(&context);
		generatePortfolios(nonModelAssets, &context);
	}

	sic::EvaluationContext &getEvaluationContext() { return context; }
};

constexpr int TraditionalAAContext::randomSeed;
std::unique_ptr<sic::TraditionalAAContext> TraditionalAAContext::singleton;

} // namespace sic

#endif // SIC_TRADITIONALAACONTEXT_H_
