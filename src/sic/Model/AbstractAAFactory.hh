#ifndef SIC_ABSTRACTAAFACTORY_H_
#define SIC_ABSTRACTAAFACTORY_H_

#include <memory>
#include <vector>

#include "sic/Model/AbstractAssetAllocation.hh"
#include "sic/Model/AbstractModelPortfolio.hh"

namespace sic {

/**
 * Abstract definition an AAFactory must follow.
 */
class AbstractAAFactory {

public:
	/**
	 * What the factory produces.
	 *
	 * Specifically, the factory must produce an AssetAllocation, and will
	 * potentially create ModelPortfolios for nodes in that AssetAllocation.
	 */
	using Result =
		std::pair<std::unique_ptr<sic::AbstractAssetAllocation>,
				  std::unique_ptr<std::vector<
					  std::unique_ptr<sic::AbstractModelPortfolio>>>>;

	/**
	 * Generate a new AssetAllocation.
	 */
	virtual Result create() = 0;

	/**
	 * Destroy the factory.
	 */
	virtual ~AbstractAAFactory() {}
};

} // namespace sic

#endif // SIC_ABSTRACTAAFACTORY_H_
