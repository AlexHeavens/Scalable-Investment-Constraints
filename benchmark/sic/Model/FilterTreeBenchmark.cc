#include <benchmark/benchmark.h>

#include "sic/Model/FilterTree.hh"
#include "sic/Model/RegularFilterTreeFactory.hh"
#include "sic/Portfolio.hh"
#include "sic/Portfolio/Asset.hh"
#include "sic/UseCase/TraditionalAAContext.hh"
#include "sic/UseCases.hh"

static std::unique_ptr<sic::TraditionalAAContext> useCase;

static void initUseCase() {
	if (!useCase) {
		useCase.reset(new sic::TraditionalAAContext());
	}
}

static void FilterTree_getLeafNode(benchmark::State &state) {

	const unsigned treeDepth = state.range(0);
	const unsigned nodeDegree = state.range(1);

	state.counters.insert(
		{{"treeDepth", treeDepth}, {"nodeDegree", nodeDegree}});

	sic::RegularFilterTreeFactory factory(treeDepth, nodeDegree);

	constexpr sic::External::ID treeID = 555;
	sic::FilterTree tree(treeID);
	factory.create(tree);

	std::vector<unsigned> path;
	path.reserve(treeDepth - 1);
	for (unsigned pathDepth = 0; pathDepth < treeDepth - 1; pathDepth++) {
		path.push_back(pathDepth % nodeDegree);
	}

	auto classes = factory.getPathClasses(path);
	constexpr sic::External::ID assetID = 123;

	sic::Asset asset(assetID, std::move(classes));

	for (auto _ : state) {
		const auto &leafNode = tree.getLeafNode(asset);
		unused(leafNode);
	}
}

BENCHMARK(FilterTree_getLeafNode)->RangeMultiplier(2)->Ranges({{1, 8}, {1, 8}});

static void FilterTree_evaluatePortfolio(benchmark::State &state) {

	const int positionCount = state.range(0);
	constexpr int positionsPerAsset = 4;
	const int assetCount = positionCount / positionsPerAsset;
	constexpr unsigned treeDepth = 4;
	constexpr unsigned nodeDegree = 4;

	state.counters.insert({{"positionCount", positionCount},
						   {"treeDepth", treeDepth},
						   {"nodeDegree", nodeDegree}});

	sic::RegularFilterTreeFactory factory(treeDepth, nodeDegree);

	constexpr sic::External::ID treeID = 555;
	sic::FilterTree tree(treeID);
	factory.create(tree);

	std::vector<sic::Asset> assets;
	assets.reserve(assetCount);

	for (int i = 0; i < assetCount; i++) {
		std::vector<unsigned> path;
		path.reserve(treeDepth - 1);
		for (unsigned pathDepth = 0; pathDepth < treeDepth - 1; pathDepth++) {
			path.push_back(pathDepth % nodeDegree);
		}

		auto classes = factory.getPathClasses(path);
		const sic::External::ID assetID = i;

		assets.emplace_back(assetID, std::move(classes));
	}

	auto positions = std::make_unique<std::vector<sic::Position>>();
	positions->reserve(positionCount);

	for (int i = 0; i < positionCount; i++) {
		sic::AbstractAsset &asset = assets.at(i % assetCount);
		constexpr sic::Value referenceValue = 10.0;
		const sic::External::ID positionID = i;

		positions->emplace_back(asset, referenceValue, positionID);
	}

	constexpr sic::External::ID portfolioID = 123;

	sic::Portfolio<> portfolio(std::move(positions), portfolioID);

	for (auto _ : state) {
		const auto &valueTree = tree.evaluate(portfolio);
		unused(valueTree);
	}
}

BENCHMARK(FilterTree_evaluatePortfolio)->RangeMultiplier(2)->Ranges({{8, 512}});

static void FilterTree_evaluatePortfolios_BankWide(benchmark::State &state) {

	initUseCase();

	auto &context = useCase->getEvaluationContext();
	std::size_t maxPortfolioCount = state.range(0);
	std::size_t threadCount = state.range(1);
	sic::ParallelParameters paraPars(threadCount);

	state.counters.insert(
		{{"portfolioCount", maxPortfolioCount}, {"threadCount", threadCount}});

	for (auto _ : state) {
		sic::UseCase::evaluateRestrictionResults(context, maxPortfolioCount,
												 paraPars);
	}
}

BENCHMARK(FilterTree_evaluatePortfolios_BankWide)
	->RangeMultiplier(2)
	->Ranges({{2 << 10, 2 << 19},
			  {1, sic::ParallelParameters::getMaxThreadCount()}});
