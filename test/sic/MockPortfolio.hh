#ifndef SIC_MOCKPORTFOLIO_H_
#define SIC_MOCKPORTFOLIO_H_

#include <gmock/gmock.h>

#include "sic/AbstractPortfolio.hh"

namespace sic {

/**
 * Mock Portfolio implementation.
 */
class MockPortfolio : public sic::AbstractPortfolio {

public:
	MOCK_METHOD0(getPositionIterators, sic::Iterators<sic::AbstractPosition>());
	MOCK_CONST_METHOD0(getPositionIterators,
					   sic::Iterators<sic::AbstractPosition>());
	MOCK_CONST_METHOD0(getTotalReferenceValue, sic::Value());
	MOCK_CONST_METHOD0(getPositionCount, std::size_t());
	MOCK_CONST_METHOD0(getAssetAllocations,
					   sic::Iterators<sic::AbstractAssetAllocation *>());
};

} // namespace sic

#endif // SIC_MOCKPORTFOLIO_H_
