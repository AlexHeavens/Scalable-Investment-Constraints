#ifndef SIC_MODEL_ALLASSETSFILTER_H_
#define SIC_MODEL_ALLASSETSFILTER_H_

#include "sic/Base/Variable.hh"
#include "sic/Model/Filter.hh"

namespace sic {

/**
 * A filter that passes for all assets.
 */
class AllAssetsFilter : public sic::Filter {

public:
	bool evaluate(const sic::AbstractAsset &asset) const override {
		unused(asset);
		return true;
	}
};

} // namespace sic

#endif // SIC_MODEL_ALLASSETSFILTER_H_
