#ifndef SIC_MODELPORTFOLIO_H_
#define SIC_MODELPORTFOLIO_H_

#include <unordered_map>

#include "sic/Base/Iterators.hh"
#include "sic/Base/Types.hh"
#include "sic/External.hh"
#include "sic/Model/AbstractModelPortfolio.hh"
#include "sic/Portfolio/AbstractAsset.hh"

namespace sic::Model {

/**
 * A simple weighted Asset list portfolio model.
 */
class ModelPortfolio : public sic::Model::AbstractModelPortfolio,
					   public sic::External {

public:
	/// A map of Assets to weights used by MPFs.
	typedef std::unordered_map<const sic::AbstractAsset *, sic::WeightRange>
		AssetWeightMap;

	/// Iterator for a ModelPortfolio's Asset/Weight pairs.
	typedef sic::Model::ModelPortfolio::AssetWeightMap::iterator
		AssetWeightIterator;

private:
	std::unique_ptr<sic::Model::ModelPortfolio::AssetWeightMap> assetWeights;

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
	ModelPortfolio(std::unique_ptr<sic::Model::ModelPortfolio::AssetWeightMap>
					   assetWeights,
				   sic::External::ID externalID);

	/**
	 * The begin and end iterators of the ModelPortfolio's Asset/Weight-Range
	 * pairs.
	 *
	 * @returns Current and end iterators.
	 */
	sic::Iterators<sic::Model::ModelPortfolio::AssetWeightIterator>
	getAssetWeightIterators();

	/**
	 * Get the number of Assets stored in the MPF.
	 */
	size_t getAssetCount() const;
};

} // namespace sic::Model

#endif // SIC_MODELPORTFOLIO_H_
