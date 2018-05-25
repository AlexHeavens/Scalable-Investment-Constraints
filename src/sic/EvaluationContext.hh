#ifndef SIC_EVALUTIONCONTEXT_H_
#define SIC_EVALUTIONCONTEXT_H_

#include "sic/ExternalCache.hh"
#include "sic/Model/AssetAllocation.hh"
#include "sic/Model/FilterTree.hh"
#include "sic/Model/ModelPortfolio.hh"
#include "sic/Portfolio.hh"
#include "sic/Portfolio/Asset.hh"

namespace sic {

/**
 * A container for the re-used objects in restriction evaluation.
 */
class EvaluationContext {

private:
	sic::ExternalCache<sic::Asset> assetCache;
	sic::ExternalCache<sic::Portfolio> portfolios;
	sic::ExternalCache<sic::FilterTree> filterTrees;
	sic::ExternalCache<sic::ModelPortfolio> modelPortfolios;
	sic::ExternalCache<sic::AssetAllocation> assetAllocations;

public:
	/**
	 * Get the Assets.
	 */
	sic::ExternalCache<sic::Asset> &getAssetCache() { return assetCache; }

	/**
	 * Get the Portfolios.
	 */
	sic::ExternalCache<sic::Portfolio> &getPortfolioCache() {
		return portfolios;
	}

	/**
	 * Get the FilterTrees.
	 */
	sic::ExternalCache<sic::FilterTree> &getFilterTreeCache() {
		return filterTrees;
	}

	/**
	 * Get the ModelPortfolios.
	 */
	sic::ExternalCache<sic::ModelPortfolio> &getModelPortfolioCache() {
		return modelPortfolios;
	}

	/**
	 * Get the AssetAllocations.
	 */
	sic::ExternalCache<sic::AssetAllocation> &getAssetAllocationCache() {
		return assetAllocations;
	}
};

} // namespace sic

#endif // SIC_EVALUTIONCONTEXT_H_
