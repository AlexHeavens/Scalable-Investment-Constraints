#ifndef SIC_MODEL_ALLASSETSFILTER_H_
#define SIC_MODEL_ALLASSETSFILTER_H_

#include "sic/Model/Filter.hh"

namespace sic::Model {

/**
 * A filter that passes for all assets.
 */
class AllAssetsFilter : public sic::Model::Filter {

public:
	bool evaluate(const sic::AbstractAsset &asset) const override {
#pragma unused(asset)
		return true;
	}
};

} // namespace sic::Model

#endif // SIC_MODEL_ALLASSETSFILTER_H_
