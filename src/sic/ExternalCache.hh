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
	std::unordered_map<sic::External::ID, std::unique_ptr<Item>> itemMap;

public:
	/**
	 * Add an item to the cache.
	 */
	void add(std::unique_ptr<Item> item) {
		itemMap.insert(std::make_pair(item->getExternalID(), std::move(item)));
	}

	/**
	 * Check if a given item exists by ID in the cache.
	 */
	bool contains(const sic::External::ID itemID) const {
		return itemMap.find(itemID) != itemMap.end();
	}

	/**
	 * Retrieve a reference to a given item by its external ID.
	 */
	Item &get(const sic::External::ID itemID) const {
		return *itemMap.at(itemID);
	}
};

} // namespace sic

#endif // SIC_EXTERNALCACHE_H_
