#ifndef SIC_MOCKMODELPORTFOLIO_H_
#define SIC_MOCKMODELPORTFOLIO_H_

#include <gmock/gmock.h>

#include "sic/Model/AbstractModelPortfolio.hh"

namespace sic {

class MockModelPortfolio : public sic::AbstractModelPortfolio {

public:
	MOCK_METHOD0(getAssetWeightIterators,
				 sic::Iterators<std::pair<const sic::AbstractAsset *const,
										  sic::WeightRange>>());
	MOCK_CONST_METHOD0(getAssetCount, std::size_t());
};

} // namespace sic

#endif // SIC_MOCKMODELPORTFOLIO_H_
