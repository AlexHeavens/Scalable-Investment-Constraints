#include "sic/AAPortfolioFactory.hh"

#include "sic/Portfolio.hh"

namespace sic {

std::unique_ptr<sic::AbstractPortfolio> AAPortfolioFactory::create() {

	auto positions = std::make_unique<std::vector<sic::Position>>();
	positions->reserve(assetToTopWeights->size());

	auto validPortfolio =
		(percentDistribution(randomGenerator) <= validPortfolioPercent);

	for (const auto &assetWeightPair : *assetToTopWeights) {
		const sic::AbstractAsset *asset = nullptr;
		const auto &toTopWeights = assetWeightPair.second;

		auto validPosition =
			(validPortfolio or
			 percentDistribution(randomGenerator) <= validPositionPercent);

		sic::Value positionRefValue = 0.0;
		if (validPosition) {
			positionRefValue = portfolioReferenceValue * toTopWeights.target;
			asset = assetWeightPair.first;
		} else {

			auto modelPosition = (percentDistribution(randomGenerator) >=
								  invalidPositionModelPercent);

			if (modelPosition) {
				positionRefValue =
					portfolioReferenceValue * toTopWeights.min - 0.01;
				asset = assetWeightPair.first;
			} else {
				positionRefValue =
					portfolioReferenceValue * toTopWeights.target;

				const std::size_t nonModelAssetIndex =
					randomGenerator() % nonModelAssets.size();
				asset = nonModelAssets.at(nonModelAssetIndex).get();
			}
		}

		const sic::External::ID positionID = nextPositionID++;

		positions->emplace_back(*asset, positionRefValue, positionID);
	}

	auto aaVector = std::make_unique<sic::Portfolio<>::AAVector>();
	aaVector->push_back(&aa);

	auto newPortfolioRawPtr = new sic::Portfolio<>(
		std::move(positions), nextPortfolioID++, std::move(aaVector));

	std::unique_ptr<sic::AbstractPortfolio> newPortfolio(newPortfolioRawPtr);

	return newPortfolio;
}

} // namespace sic
