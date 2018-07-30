#ifndef SIC_CACHE_H_
#define SIC_CACHE_H_

#include <boost/optional.hpp>

namespace sic {

/**
 * Common interface for a cache with a custom index type.
 */
template <typename KeyType, typename ValueType> class Cache {

public:
	/**
	 * Retrieve a specific item from the cache.
	 */
	virtual boost::optional<ValueType> get(const KeyType key) const = 0;

	/**
	 * Add a new item to the cache.
	 *
	 * Depending on the cache's implementation, this may eject an existing item.
	 */
	virtual void add(KeyType key, ValueType value) = 0;
};

} // namespace sic

#endif // SIC_CACHE_H_
