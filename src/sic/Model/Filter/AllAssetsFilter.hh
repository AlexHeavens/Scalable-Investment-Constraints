#ifndef SIC_MODEL_ALLASSETSFILTER_H_
#define SIC_MODEL_ALLASSETSFILTER_H_

#include "sic/Model/Filter.hh"

namespace sic::Model {

/**
 * A filter that passes for all assets.
 */
class AllAssetsFilter : public sic::Model::Filter {

private:
	static bool passAllAssets(const sic::Asset &asset) {
#pragma unused(asset)
		return true;
	}

public:
	/**
	 * Create a filter that passes for all assets.
	 */
	AllAssetsFilter()
		: sic::Model::Filter(sic::Model::AllAssetsFilter::passAllAssets) {}
};

} // namespace sic::Model

#endif // SIC_MODEL_ALLASSETSFILTER_H_
