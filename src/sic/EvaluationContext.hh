#ifndef SIC_EVALUTIONCONTEXT_H_
#define SIC_EVALUTIONCONTEXT_H_

#include "sic/AbstractPortfolio.hh"
#include "sic/ExternalCache.hh"
#include "sic/Model/AbstractAssetAllocation.hh"
#include "sic/Model/AbstractFilterTree.hh"
#include "sic/Model/AbstractModelPortfolio.hh"
#include "sic/Portfolio/AbstractAsset.hh"

namespace sic {

/**
 * A container for the re-used objects in restriction evaluation.
 */
class EvaluationContext {

private:
	sic::ExternalCache<sic::AbstractAsset> assetCache;
	sic::ExternalCache<sic::AbstractPortfolio> portfolios;
	sic::ExternalCache<sic::AbstractFilterTree> filterTrees;
	sic::ExternalCache<sic::AbstractModelPortfolio> modelPortfolios;
	sic::ExternalCache<sic::AbstractAssetAllocation> assetAllocations;

public:
	/**
	 * Get the Assets.
	 */
	sic::ExternalCache<sic::AbstractAsset> &getAssetCache() {
		return assetCache;
	}

	/**
	 * Get the Portfolios.
	 */
	sic::ExternalCache<sic::AbstractPortfolio> &getPortfolioCache() {
		return portfolios;
	}

	/**
	 * Get the FilterTrees.
	 */
	sic::ExternalCache<sic::AbstractFilterTree> &getFilterTreeCache() {
		return filterTrees;
	}

	/**
	 * Get the ModelPortfolios.
	 */
	sic::ExternalCache<sic::AbstractModelPortfolio> &getModelPortfolioCache() {
		return modelPortfolios;
	}

	/**
	 * Get the AssetAllocations.
	 */
	sic::ExternalCache<sic::AbstractAssetAllocation> &
	getAssetAllocationCache() {
		return assetAllocations;
	}
};

} // namespace sic

#endif // SIC_EVALUTIONCONTEXT_H_
