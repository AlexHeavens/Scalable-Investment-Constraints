#ifndef SIC_MOCKVALUETREE_H_
#define SIC_MOCKVALUETREE_H_

#include <gmock/gmock.h>

#include "sic/Portfolio/AbstractValueTree.hh"

namespace sic {

/**
 * Mocked ValueTree API for testing.
 */
class MockValueTree : public sic::AbstractValueTree {

public:
	MOCK_CONST_METHOD1(getNodeWeight,
					   sic::Weight(const sic::AbstractFilterNode &filterNode));
	bool operator==(const sic::AbstractValueTree &other) override {
		unused(other);
		return false;
	}
	MOCK_CONST_METHOD0(getNodeWeightIterators,
					   sic::Iterators<NodeWeightMap::value_type>());
};

} // namespace sic

#endif // SIC_MOCKVALUETREE_H_
