#include <benchmark/benchmark.h>

#include "sic/Model/RegularFilterTreeFactory.hh"
#include "sic/Portfolio/Asset.hh"

static void FilterTree_getLeafNode(benchmark::State &state) {

	const unsigned depth = state.range(0);
	const unsigned nodeDegree = state.range(1);
	sic::RegularFilterTreeFactory factory(depth, nodeDegree);

	constexpr sic::External::ID treeID = 555;
	sic::FilterTree tree(treeID);
	factory.create(tree);

	std::vector<unsigned> path;
	path.reserve(depth - 1);
	for (unsigned pathDepth = 0; pathDepth < depth - 1; pathDepth++) {
		path.push_back(pathDepth % nodeDegree);
	}

	auto classes = factory.getPathClasses(path);
	constexpr sic::External::ID assetID = 123;

	sic::Asset asset(assetID, std::move(classes));

	for (auto _ : state) {

		const auto &leafNode = tree.getLeafNode(asset);
#pragma unused(leafNode)
	}
}

BENCHMARK(FilterTree_getLeafNode)->RangeMultiplier(2)->Ranges({{1, 8}, {1, 8}});
