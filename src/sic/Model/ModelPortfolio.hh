#ifndef SIC_MODELPORTFOLIO_H_
#define SIC_MODELPORTFOLIO_H_

#include "sic/External.hh"
#include "sic/Model/AbstractModelPortfolio.hh"

namespace sic {

/**
 * A simple weighted Asset list portfolio model.
 */
class ModelPortfolio : public sic::AbstractModelPortfolio,
					   public sic::External {

private:
	std::unique_ptr<sic::AbstractAsset::AssetWeightMap> assetWeights;

public:
	/**
	 * Create an MPF with a given list of weigh-ranged Assets.
	 *
	 * @param assetWeights Non empty weight-range list of Assets the MPF will
	 * take ownership of.  An Asset cannot appear twice in the list and all
	 * target weights must sum to 1.0, within a tolerance of
	 * sic::Tolerance<sic::Weight>().
	 * @param externalID ID of the MPF in the external system.
	 */
	ModelPortfolio(
		std::unique_ptr<sic::AbstractAsset::AssetWeightMap> assetWeights,
		sic::External::ID externalID);

	sic::Iterators<std::pair<const sic::AbstractAsset *const, sic::WeightRange>>
	getAssetWeightIterators() const override;

	std::size_t getAssetCount() const override { return assetWeights->size(); }
};

} // namespace sic

#endif // SIC_MODELPORTFOLIO_H_
