#ifndef SIC_AAPORTFOLIOFACTORY_H_
#define SIC_AAPORTFOLIOFACTORY_H_

#include "sic/AbstractPortfolioFactory.hh"
#include "sic/Model/AbstractAssetAllocation.hh"

namespace sic {

/**
 * A factory for constructing Portfolios according to an AssetAllocation model.
 *
 * The Portfolios generated from this will have Asset weights that correspond
 * to the weights given by a specific AssetAllocation.
 */
class AAPortfolioFactory : public sic::AbstractPortfolioFactory {

public:
	static constexpr float defaultValidPortfolioRate = 1.0;
	static constexpr int defaultInvalidPositionCount = 0;

private:
	sic::AbstractAssetAllocation &aa;
	const sic::Value portfolioReferenceValue;
	sic::External::ID nextPortfolioID, nextPositionID;
	std::unique_ptr<sic::AbstractAsset::AssetWeightMap> assetToTopWeights;

	static constexpr int randomSeed = 232434;
	std::mt19937 randomGenerator;

	// Distribution for 1% granularity.
	std::uniform_int_distribution<int> percentDistribution;
	const int validPortfolioPercent;
	const int invalidPositionCount;
	const std::vector<std::unique_ptr<sic::AbstractAsset>> &nonModelAssets;

public:
	/**
	 * Create a new factory.
	 *
	 * @param aa AssetAllocation to use as a template for the portfolio.
	 */
	AAPortfolioFactory(sic::AbstractAssetAllocation &aa,
					   const sic::Value portfolioReferenceValue,
					   sic::External::ID initialPortfolioID = 0,
					   sic::External::ID initialPositionID = 0,
					   float validPortfolioRate = defaultValidPortfolioRate,
					   int invalidPositionCount = defaultInvalidPositionCount,
					   const std::vector<std::unique_ptr<sic::AbstractAsset>>
						   &nonModelAssets = {})
		: aa(aa), portfolioReferenceValue(portfolioReferenceValue),
		  nextPortfolioID(initialPortfolioID),
		  nextPositionID(initialPositionID), percentDistribution(0, 100),
		  validPortfolioPercent(static_cast<int>(100.0 * validPortfolioRate)),
		  invalidPositionCount(invalidPositionCount),
		  nonModelAssets(nonModelAssets) {

		auto toTopWeights = aa.getAssetToTopWeights();
		assetToTopWeights = std::move(toTopWeights);
		randomGenerator.seed(randomSeed);
	}

	std::unique_ptr<sic::AbstractPortfolio> create() override;
};

} // namespace sic

#endif // SIC_AAPORTFOLIOFACTORY_H_
