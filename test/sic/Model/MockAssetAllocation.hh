#ifndef SIC_MOCKASSETALLOCATION_H_
#define SIC_MOCKASSETALLOCATION_H_

#include <gmock/gmock.h>

#include "sic/Model/AbstractAssetAllocation.hh"

namespace sic {

/**
 * Mock AssetAllocation for testing.
 */
class MockAssetAllocation : public sic::AbstractAssetAllocation {

public:
	MockAssetAllocation(sic::External::ID externalID = 0)
		: sic::AbstractAssetAllocation(externalID) {}
	MOCK_CONST_METHOD0(getFilterTree, const sic::AbstractFilterTree &());
	MOCK_CONST_METHOD1(getAANode,
					   const sic::AbstractAssetAllocationNode &(
						   const sic::AbstractFilterNode &filterNode));
	MOCK_CONST_METHOD0(getAANodeIterators,
					   sic::Iterators<FilterNodeMap::value_type>());
};

} // namespace sic

#endif // SIC_MOCKASSETALLOCATION_H_
