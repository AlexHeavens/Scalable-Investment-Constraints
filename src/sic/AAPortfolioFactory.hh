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

private:
	const sic::AbstractAssetAllocation &aa;
	const sic::Value portfolioReferenceValue;
	sic::External::ID nextPortfolioID, nextPositionID;
	std::unique_ptr<sic::AbstractAsset::AssetWeightMap> assetToTopWeights;

public:
	/**
	 * Create a new factory.
	 *
	 * @param aa AssetAllocation to use as a template for the portfolio.
	 */
	AAPortfolioFactory(const sic::AbstractAssetAllocation &aa,
					   const sic::Value portfolioReferenceValue,
					   sic::External::ID initialPortfolioID = 0,
					   sic::External::ID initialPositionID = 0)
		: aa(aa), portfolioReferenceValue(portfolioReferenceValue),
		  nextPortfolioID(initialPortfolioID),
		  nextPositionID(initialPositionID) {

		auto toTopWeights = aa.getAssetToTopWeights();
		assetToTopWeights = std::move(toTopWeights);
	}

	std::unique_ptr<sic::AbstractPortfolio> create() override;
};

} // namespace sic

#endif // SIC_AAPORTFOLIOFACTORY_H_
