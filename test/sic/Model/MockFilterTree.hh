#ifndef SIC_MOCKFILTERTREE_H_
#define SIC_MOCKFILTERTREE_H_

#include <gmock/gmock.h>

#include "sic/Base/Variable.hh"
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
	MOCK_CONST_METHOD1(
		evaluateRaw,
		sic::AbstractValueTree *(const sic::AbstractPortfolio &portfolio));
	MOCK_CONST_METHOD0(begin_nodes, node_iterator());
	MOCK_CONST_METHOD0(end_nodes, node_iterator());

	/**
	 * GMock-compatible wrapper for evaluate.
	 */
	std::unique_ptr<sic::AbstractValueTree> evaluate(
		const sic::AbstractPortfolio &portfolio,
		boost::optional<AssetLeafNodeCache *> leafNodeCache) const override {
		unused(leafNodeCache);
		auto *valueTreeRawPtr = evaluateRaw(portfolio);
		return std::unique_ptr<sic::AbstractValueTree>(valueTreeRawPtr);
	}
};

} // namespace sic

#endif // SIC_MOCKFILTERTREE_H_
