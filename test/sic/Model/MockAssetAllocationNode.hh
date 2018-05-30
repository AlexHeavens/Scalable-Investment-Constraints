#ifndef SIC_MOCKASSETALLOCATIONNODE_H_
#define SIC_MOCKASSETALLOCATIONNODE_H_

#include <gmock/gmock.h>

#include "sic/Model/AbstractAssetAllocationNode.hh"

namespace sic {

/**
 * Mock AssetAllocationNode for testing.
 */
class MockAssetAllocationNode : public sic::AbstractAssetAllocationNode {

public:
	MOCK_CONST_METHOD0(getWeightRange, const sic::WeightRange &());
};

} // namespace sic

#endif // SIC_MOCKASSETALLOCATIONNODE_H_
