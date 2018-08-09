#ifndef SIC_COLLECTION_H_
#define SIC_COLLECTION_H_

#include <vector>

#include "sic/Base/Source.hh"

namespace sic {

/**
 * Wrapper class, proving a generic source of items from an existing,
 * non-inhertable object.
 *
 * This wrapper is useful for providing generic Source objects of standard
 * container types (e.g. vector, map).
 *
 * @param ItemType the type of Item stored in the Collection.
 * @param
 */
template <typename ItemType, typename CollectionType>
class Collection : public sic::Source<ItemType> {

protected:
	CollectionType &collection;

public:
	/**
	 * Construct a Collection wrapper.
	 */
	Collection(CollectionType &collection) : collection(collection) {}

	typename sic::Iterators<ItemType> getItems() const override {
		return sic::Iterators<ItemType>(collection);
	}

	std::size_t size() const override { return collection.size(); }
};

/**
 * Vector-specific collection.
 *
 * This takes advantage of the constant time lookup a vector gives.
 */
template <typename ItemType>
class VectorCollection
	: public sic::Collection<ItemType, std::vector<ItemType>> {

public:
	/**
	 * Construct a VectorCollection wrapper.
	 */
	VectorCollection(std::vector<ItemType> &collection)
		: sic::Collection<ItemType, std::vector<ItemType>>(collection) {}

	virtual ItemType &at(const std::size_t &index) const override {
		return sic::Collection<ItemType, std::vector<ItemType>>::collection.at(
			index);
	}
};

} // namespace sic

#endif // SIC_COLLECTION_H_
