#include "sic/AssetCache.hh"

void sic::AssetCache::add(const sic::AbstractAsset &asset) {
	assetMap.insert({asset.getExternalID(), asset});
}

bool sic::AssetCache::contains(const sic::External::ID assetID) const {
	return assetMap.find(assetID) != assetMap.end();
}

const sic::AbstractAsset &
sic::AssetCache::get(const sic::External::ID assetID) const {
	return assetMap.at(assetID);
}
