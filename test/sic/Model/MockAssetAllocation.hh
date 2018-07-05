#ifndef SIC_MOCKASSETALLOCATION_H_
#define SIC_MOCKASSETALLOCATION_H_

#include <gmock/gmock.h>

#include "sic/Model/AbstractAssetAllocation.hh"

namespace sic {

/**
 * Mock AssetAllocation for testing.
 */
class MockAssetAllocation : public sic::AbstractAssetAllocation {

public:
	MockAssetAllocation(sic::External::ID externalID = 0)
		: sic::AbstractAssetAllocation(externalID) {}
	MOCK_CONST_METHOD0(getFilterTree, const sic::AbstractFilterTree &());
	MOCK_CONST_METHOD1(getAANode,
					   const sic::AbstractAssetAllocationNode &(
						   const sic::AbstractFilterNode &filterNode));
	MOCK_CONST_METHOD0(getAANodeIterators,
					   sic::Iterators<FilterNodeMap::value_type>());
	MOCK_CONST_METHOD1(
		generateRestrictionResults,
		std::unique_ptr<ResultVector>(const sic::AbstractPortfolio &portfolio));
	MOCK_CONST_METHOD0(getAssetToTopWeightsRaw,
					   sic::AbstractAsset::AssetWeightMap *());

	std::unique_ptr<sic::AbstractAsset::AssetWeightMap>
	getAssetToTopWeights() const override {
		std::unique_ptr<sic::AbstractAsset::AssetWeightMap> assetToTopWeights(
			getAssetToTopWeightsRaw());
		return assetToTopWeights;
	}
};

} // namespace sic

#endif // SIC_MOCKASSETALLOCATION_H_
