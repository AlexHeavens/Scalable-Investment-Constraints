#ifndef SIC_MOCKFILTERTREE_H_
#define SIC_MOCKFILTERTREE_H_

#include <gmock/gmock.h>

#include "sic/Model/AbstractFilterTree.hh"

namespace sic {

/**
 * Mocked FilterTree for testing.
 */
class MockFilterTree : public sic::AbstractFilterTree {

public:
	MockFilterTree() : sic::AbstractFilterTree(1) {}

	MOCK_CONST_METHOD1(getLeafNode, const sic::AbstractFilterNode &(
										const sic::AbstractAsset &asset));

	MOCK_METHOD0(getRootNode, sic::AbstractFilterNode &());
	MOCK_CONST_METHOD0(getRootNode, sic::AbstractFilterNode &());
	MOCK_CONST_METHOD1(evaluate, std::unique_ptr<sic::AbstractValueTree>(
									 const sic::AbstractPortfolio &portfolio));
};

} // namespace sic

#endif // SIC_MOCKFILTERTREE_H_
