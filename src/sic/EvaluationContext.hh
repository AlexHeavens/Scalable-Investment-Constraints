#ifndef SIC_EVALUTIONCONTEXT_H_
#define SIC_EVALUTIONCONTEXT_H_

#include "sic/ExternalCache.hh"
#include "sic/Portfolio/Asset.hh"

namespace sic {

/**
 * A container for the re-used objects in restriction evaluation.
 */
class EvaluationContext {

private:
	sic::ExternalCache<sic::Asset> assetCache;

public:
	/**
	 * Get the AssetCache.
	 */
	sic::ExternalCache<sic::Asset> &getAssetCache() { return assetCache; }
};

} // namespace sic

#endif // SIC_EVALUTIONCONTEXT_H_
