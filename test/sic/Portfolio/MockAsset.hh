#ifndef SIC_MOCKASSET_H_
#define SIC_MOCKASSET_H_

#include <gmock/gmock.h>

#include "sic/Portfolio/AbstractAsset.hh"

namespace sic {

/**
 * Mocked Asset API for testing.
 */
class MockAsset : public sic::AbstractAsset {

public:
	MockAsset(sic::External::ID externalID = 0)
		: sic::AbstractAsset(externalID) {}
	explicit MockAsset(const sic::MockAsset &other)
		: sic::AbstractAsset(other) {}
	MOCK_CONST_METHOD1(hasClass, bool(sic::AbstractAsset::Class));
};

} // namespace sic

#endif // SIC_MOCKASSET_H_
