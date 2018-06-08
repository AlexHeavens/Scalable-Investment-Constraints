#ifndef SIC_ABSTRACTASSETALLOCATION_H_
#define SIC_ABSTRACTASSETALLOCATION_H_

#include "sic/Model/AbstractFilterTree.hh"

namespace sic {

/**
 * Interface of an AssetAllocation.
 */
class AbstractAssetAllocation {

public:
	/**
	 * The FilterTree representing the AA's structure.
	 */
	virtual const sic::AbstractFilterTree &getFilterTree() const = 0;

	/**
	 * Destroy the AssetAlloction.
	 */
	virtual ~AbstractAssetAllocation() {}
};

} // namespace sic

#endif // SIC_ABSTRACTASSETALLOCATION_H_
