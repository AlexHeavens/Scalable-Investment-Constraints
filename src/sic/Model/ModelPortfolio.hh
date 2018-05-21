#ifndef SIC_MODELPORTFOLIO_H_
#define SIC_MODELPORTFOLIO_H_

#include <memory>
#include <unordered_map>

#include "sic/Base/Types.hh"
#include "sic/External.hh"
#include "sic/Portfolio/Asset.hh"

namespace sic::Model {

/**
 * A simple weighted Asset list portfolio model.
 */
class ModelPortfolio : public sic::External {

public:
	/// A map of Assets to weights used by MPFs.
	typedef std::unordered_map<std::shared_ptr<sic::Asset>, sic::Weight>
		AssetWeightMap;

private:
	std::unique_ptr<sic::Model::ModelPortfolio::AssetWeightMap> assetWeights;

public:
	/**
	 * Create an MPF with a given list of weighted Assets.
	 *
	 * @param assetWeights Weighted list of Assets the MPF will take ownership
	 * of.  This is a list of shared pointers to ensure calculations based on
	 * the  MPF remain valid if the AssetCache is destroyed.
	 * @param externalID ID of the MPF in the external system.
	 */
	ModelPortfolio(std::unique_ptr<sic::Model::ModelPortfolio::AssetWeightMap>
					   assetWeights,
				   sic::External::ID externalID)
		: sic::External(externalID), assetWeights(std::move(assetWeights)) {}

	/**
	 * Get the number of Assets stored in the MPF.
	 */
	size_t getAssetCount() const;
};

} // namespace sic::Model

#endif // SIC_MODELPORTFOLIO_H_
