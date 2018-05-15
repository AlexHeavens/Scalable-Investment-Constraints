#ifndef SIC_MODEL_ASSETCLASSFILTER_H_
#define SIC_MODEL_ASSETCLASSFILTER_H_

#include "sic/Model/Filter.hh"
#include "sic/Portfolio/Asset.hh"

namespace sic::Model {

/**
 * Filter of Assets of a specific class.
 *
 * @see sic::Asset::Class
 */
class AssetClassFilter : public sic::Model::Filter {

private:
	const sic::Asset::Class assetClass;

public:
	/**
	 * Construct an Asset Class-based filter.
	 *
	 * @param assetClass class to filter by.
	 */
	AssetClassFilter(sic::Asset::Class assetClass) : assetClass(assetClass) {}

	bool evaluate(const sic::Asset &asset) const override;
};

} // namespace sic::Model

#endif // SIC_MODEL_ASSETCLASSFILTER_H_
