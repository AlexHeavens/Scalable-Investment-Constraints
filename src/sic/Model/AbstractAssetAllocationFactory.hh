#ifndef SIC_ABSTRACTASSETALLOCATIONFACTORY_H_
#define SIC_ABSTRACTASSETALLOCATIONFACTORY_H_

#include <memory>

#include "sic/Model/AbstractAssetAllocation.hh"

namespace sic {

/**
 * Abstracted interface for a factory of AssetAllocations.
 */
class AbstractAssetAllocationFactory {

public:
	/**
	 * Construct an AssetAllocation and return to the caller with ownership as a
	 * unique_ptr.
	 */
	virtual std::unique_ptr<sic::AbstractAssetAllocation> create() = 0;
};

} // namespace sic

#endif // SIC_ABSTRACTASSETALLOCATIONFACTORY_H_
