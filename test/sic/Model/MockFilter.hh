#ifndef SIC_MOCKFILTER_H_
#define SIC_MOCKFILTER_H_

#include <gmock/gmock.h>

#include "sic/Portfolio/AbstractAsset.hh"

namespace sic {

class MockFilter : public sic::Filter {

public:
	MOCK_CONST_METHOD1(evaluate, bool(const sic::AbstractAsset &));
};

} // namespace sic

#endif // SIC_MOCKFILTER_H_
