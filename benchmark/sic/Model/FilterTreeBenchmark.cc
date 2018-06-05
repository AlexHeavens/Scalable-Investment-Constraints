#include <benchmark/benchmark.h>

#include "sic/Model/RegularFilterTreeFactory.hh"
#include "sic/Portfolio/Asset.hh"

static void FilterTree_getLeafNode(benchmark::State &state) {

	constexpr unsigned depth = 4;
	constexpr unsigned nodeDegree = 5;
	sic::RegularFilterTreeFactory factory(depth, nodeDegree);

	constexpr sic::External::ID treeID = 555;
	sic::FilterTree tree(treeID);
	factory.create(tree);

	const std::vector<unsigned> path{1, 3, 5};
	auto classes = factory.getPathClasses(path);
	constexpr sic::External::ID assetID = 123;

	sic::Asset asset(assetID, std::move(classes));

	for (auto _ : state) {

		const auto &leafNode = tree.getLeafNode(asset);
#pragma unused(leafNode)
	}
}

BENCHMARK(FilterTree_getLeafNode);
