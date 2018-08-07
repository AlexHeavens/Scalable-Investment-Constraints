#ifndef SIC_AAPORTFOLIOFACTORY_H_
#define SIC_AAPORTFOLIOFACTORY_H_

#include <random>
#include <vector>

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
	static constexpr float defaultValidPositionRate = 0.9;

	/// For invalid portfolios, the ratio of invalid positions that are still
	/// part of the model, as opposed to off-model positions.
	static constexpr float invalidPortfolioPositionModelRatio = 0.5;

private:
	const sic::AbstractAssetAllocation &aa;
	const sic::Value portfolioReferenceValue;
	sic::External::ID nextPortfolioID, nextPositionID;
	std::unique_ptr<sic::AbstractAsset::AssetWeightMap> assetToTopWeights;

	static constexpr int randomSeed = 232434;
	std::random_device randomDevice;
	std::mt19937 randomGenerator;

	// Distribution for 1% granularity.
	std::uniform_int_distribution<int> percentDistribution;
	const int validPortfolioPercent;
	const int validPositionPercent;
	static const int invalidPositionModelPercent =
		static_cast<int>(100.0 * invalidPortfolioPositionModelRatio);
	const std::vector<std::unique_ptr<sic::AbstractAsset>> &nonModelAssets;

public:
	/**
	 * Create a new factory.
	 *
	 * @param aa AssetAllocation to use as a template for the portfolio.
	 */
	AAPortfolioFactory(const sic::AbstractAssetAllocation &aa,
					   const sic::Value portfolioReferenceValue,
					   sic::External::ID initialPortfolioID = 0,
					   sic::External::ID initialPositionID = 0,
					   float validPortfolioRate = defaultValidPortfolioRate,
					   float validPositionRate = defaultValidPositionRate,
					   const std::vector<std::unique_ptr<sic::AbstractAsset>>
						   &nonModelAssets = {})
		: aa(aa), portfolioReferenceValue(portfolioReferenceValue),
		  nextPortfolioID(initialPortfolioID),
		  nextPositionID(initialPositionID), randomDevice(),
		  randomGenerator(randomSeed), percentDistribution(0, 100),
		  validPortfolioPercent(static_cast<int>(100.0 * validPortfolioRate)),
		  validPositionPercent(static_cast<int>(100.0 * validPositionRate)),
		  nonModelAssets(nonModelAssets) {

		auto toTopWeights = aa.getAssetToTopWeights();
		assetToTopWeights = std::move(toTopWeights);
	}

	std::unique_ptr<sic::AbstractPortfolio> create() override;
};

} // namespace sic

#endif // SIC_AAPORTFOLIOFACTORY_H_
