#include "sic/AAPortfolioFactory.hh"

#include "sic/Portfolio.hh"

namespace sic {

void AAPortfolioFactory::initialiseToTopWeights(
	sic::AbstractAsset::AssetWeightMap *assetToTopWeights) const {

	auto aaIts = aa.getAANodeIterators();
	while (aaIts.remaining()) {

		auto &filterAANodePair = *aaIts.current();
		auto &aaNode = *filterAANodePair.second;
		aaNode.attachToTopAssetWeights(assetToTopWeights);

		aaIts.current()++;
	}
}

std::unique_ptr<sic::AbstractPortfolio> AAPortfolioFactory::create() {

	sic::AbstractAsset::AssetWeightMap assetToTopWeights;
	initialiseToTopWeights(&assetToTopWeights);

	auto positions = std::make_unique<std::vector<sic::Position>>();
	positions->reserve(assetToTopWeights.size());

	for (const auto &assetWeightPair : assetToTopWeights) {
		const sic::AbstractAsset &asset = *(assetWeightPair.first);
		const auto &toTopWeights = assetWeightPair.second;
		const sic::Value positionRefValue =
			portfolioReferenceValue * toTopWeights.target;
		const sic::External::ID positionID = nextPositionID++;

		positions->emplace_back(asset, positionRefValue, positionID);
	}

	auto aaVector = std::make_unique<sic::Portfolio<>::AAVector>();
	aaVector->push_back(&aa);

	auto newPortfolioRawPtr = new sic::Portfolio<>(
		std::move(positions), nextPortfolioID++, std::move(aaVector));

	std::unique_ptr<sic::AbstractPortfolio> newPortfolio(newPortfolioRawPtr);

	return newPortfolio;
}

} // namespace sic
