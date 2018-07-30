#ifndef SIC_SIMPLECACHE_H_
#define SIC_SIMPLECACHE_H_

#include <unordered_map>

#include "sic/Base/Cache.hh"

namespace sic {

/**
 * A simple, ever-increasing cache.
 */
template <typename KeyType, typename ValueType>
class SimpleCache : public sic::Cache<KeyType, ValueType> {

private:
	std::unordered_map<KeyType, ValueType> cacheValues;

public:
	boost::optional<ValueType> get(const KeyType key) const override {
		boost::optional<ValueType> value;

		auto valueLookup = cacheValues.find(key);
		if (valueLookup != cacheValues.end()) {
			value = valueLookup->second;
		}
		return value;
	}

	void add(KeyType key, ValueType value) override {
		cacheValues.insert(std::make_pair(key, value));
	}
};

} // namespace sic

#endif // SIC_SIMPLECACHE_H_
