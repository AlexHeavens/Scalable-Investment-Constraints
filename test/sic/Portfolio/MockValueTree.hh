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
};

} // namespace sic

#endif // SIC_MOCKVALUETREE_H_
