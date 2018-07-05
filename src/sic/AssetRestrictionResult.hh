#ifndef SIC_ASSETRESTRICTIONRESULT_H_
#define SIC_ASSETRESTRICTIONRESULT_H_

#include "sic/Base/Types.hh"
#include "sic/Portfolio/AbstractAsset.hh"
#include "sic/RestrictionResult.hh"

namespace sic {

/**
 * Result of if an Asset is in line with its client's model.
 */
class AssetRestrictionResult : public sic::RestrictionResult {

private:
	const sic::AbstractAsset &asset;
	const sic::Weight toTopWeight;
	const sic::WeightRange weightRange;

public:
	/**
	 * Construct an Asset result.
	 */
	AssetRestrictionResult(const sic::AbstractAsset &asset,
						   const sic::Weight toTopWeight,
						   const sic::WeightRange weightRange)
		: asset(asset), toTopWeight(toTopWeight), weightRange(weightRange) {}

	std::string serialise() const override;

	State getState() const override {

		auto state = sic::RestrictionResult::State::Fail;
		if (toTopWeight >= weightRange.min and toTopWeight <= weightRange.max) {
			state = sic::RestrictionResult::State::Pass;
		}

		return state;
	}

	bool operator==(const sic::RestrictionResult &other) const override {
		auto assetResult =
			dynamic_cast<const sic::AssetRestrictionResult *>(&other);

		bool match = false;
		if (assetResult != nullptr) {
			match = &asset == &assetResult->asset and
					toTopWeight == assetResult->toTopWeight and
					weightRange == assetResult->weightRange;
		}

		return match;
	}
}; // namespace sic

} // namespace sic

#endif // SIC_ASSETRESTRICTIONRESULT_H_
