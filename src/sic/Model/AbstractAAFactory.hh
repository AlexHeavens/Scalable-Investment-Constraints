#ifndef SIC_ABSTRACTAAFACTORY_H_
#define SIC_ABSTRACTAAFACTORY_H_

#include <memory>

#include "sic/Model/AbstractAssetAllocation.hh"

namespace sic {

/**
 * Abstract definition an AAFactory must follow.
 */
class AbstractAAFactory {

public:
	virtual std::unique_ptr<sic::AbstractAssetAllocation> create() = 0;
	virtual ~AbstractAAFactory() {}
};

} // namespace sic

#endif // SIC_ABSTRACTAAFACTORY_H_
