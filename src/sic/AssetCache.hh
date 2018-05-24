#ifndef SIC_ASSET_CACHE_H_
#define SIC_ASSET_CACHE_H_

#include <unordered_map>

#include "sic/Portfolio/AbstractAsset.hh"

namespace sic {

/**
 * An optimised container for the Assets available to a given
 * EvaluationContext.
 */
class AssetCache {

private:
	std::unordered_map<sic::External::ID, sic::AbstractAsset> assetMap;

public:
	/**
	 * Add an Asset to the cache.
	 *
	 * Note: this Asset will be copied by value to avoid the latency of pointer
	 * indirection necessary if we were moving the Asset.
	 */
	void add(const sic::AbstractAsset &asset);

	/**
	 * Check if a given Assets exists by ID in the cache.
	 */
	bool contains(const sic::External::ID assetID) const;

	/**
	 * Retrieve a reference to a given Asset by its external ID.
	 */
	const sic::AbstractAsset &get(const sic::External::ID assetID) const;
};

} // namespace sic

#endif // SIC_ASSET_CACHE_H_
