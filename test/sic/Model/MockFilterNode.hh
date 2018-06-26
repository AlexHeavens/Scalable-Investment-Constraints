#ifndef SIC_MOCKFILTERNODE_H_
#define SIC_MOCKFILTERNODE_H_

#include <gmock/gmock.h>

#include "sic/Model/AbstractFilterNode.hh"

namespace sic {

/**
 * Mocked FilterNode for testing.
 */
class MockFilterNode : public sic::AbstractFilterNode {

public:
	/**
	 * Gmock-compatible version of addChild.
	 *
	 * GMock cannot mock non-copyable parameters, so this function acts as a
	 * usable wrapper around the mocked, raw pointer GMock version.
	 */
	sic::AbstractFilterNode &
	addChild(std::unique_ptr<const sic::Filter> childFilter) override {
		return addChild(childFilter.get());
	}

	MOCK_METHOD1(addChild, sic::AbstractFilterNode &(const sic::Filter *));
	MOCK_CONST_METHOD0(getParentNode, const sic::AbstractFilterNode *());
	MOCK_CONST_METHOD0(getFilter, const sic::Filter &());
	MOCK_CONST_METHOD1(filterToChild, const sic::AbstractFilterNode *(
										  const sic::AbstractAsset &asset));
	MOCK_METHOD0(getChildIterators,
				 sic::Iterators<std::unique_ptr<sic::AbstractFilterNode>>());
	MOCK_CONST_METHOD0(
		getChildIterators,
		sic::Iterators<std::unique_ptr<sic::AbstractFilterNode>>());
	MOCK_CONST_METHOD0(getChildCount, std::size_t());
	MOCK_CONST_METHOD0(getFilterTree, const sic::AbstractFilterTree &());
	MOCK_CONST_METHOD0(getParentIndex, std::size_t());
	MOCK_CONST_METHOD1(getChild, sic::AbstractFilterNode &(std::size_t));
};

} // namespace sic

#endif // SIC_MOCKFILTERNODE_H_
