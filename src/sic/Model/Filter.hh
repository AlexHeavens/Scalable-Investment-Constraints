#ifndef SIC_MODEL_FILTER_H_
#define SIC_MODEL_FILTER_H_

#include "sic/Portfolio/AbstractAsset.hh"

namespace sic::Model {

/**
 * Asset Filter for placement in a FilterTree.
 */
class Filter {

public:
	/**
	 * If a given Asset passes the filter.
	 */
	virtual bool evaluate(const sic::AbstractAsset &asset) const = 0;

	virtual ~Filter(){};
};

} // namespace sic::Model

#endif // SIC_MODEL_FILTER_H_
