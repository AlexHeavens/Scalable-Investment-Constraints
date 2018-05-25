#ifndef SIC_EXTERNALCACHE_H_
#define SIC_EXTERNALCACHE_H_

#include <unordered_map>

#include "sic/External.hh"

namespace sic {

/**
 * An optimised container for a type of External objects available to a given
 * EvaluationContext.
 */
template <typename Item> class ExternalCache {

private:
	std::unordered_map<sic::External::ID, Item> itemMap;

public:
	/**
	 * Add an item to the cache.
	 *
	 * Note: this item will be copied by value to avoid the latency of pointer
	 * indirection necessary if we were only moving it.
	 */
	void add(const Item &item) { itemMap.insert({item.getExternalID(), item}); }

	/**
	 * Check if a given item exists by ID in the cache.
	 */
	bool contains(const sic::External::ID itemID) const {
		return itemMap.find(itemID) != itemMap.end();
	}

	/**
	 * Retrieve a reference to a given item by its external ID.
	 */
	const Item &get(const sic::External::ID itemID) const {
		return itemMap.at(itemID);
	}
};

} // namespace sic

#endif // SIC_EXTERNALCACHE_H_
