#ifndef SIC_MOCKPOSITION_H_
#define SIC_MOCKPOSITION_H_

#include <gmock/gmock.h>

#include "sic/Portfolio/AbstractPosition.hh"

namespace sic {

class MockPosition : public sic::AbstractPosition {

public:
	MockPosition() = default;
	MockPosition(sic::External::ID externalID)
		: sic::AbstractPosition(externalID) {}

	MockPosition(const MockPosition &rhs) : sic::AbstractPosition(rhs) {}

	MOCK_CONST_METHOD0(getAsset, sic::AbstractAsset &());
	MOCK_CONST_METHOD0(getReferenceValue, sic::Value());
};

} // namespace sic

#endif // SIC_MOCKPOSITION_H_
