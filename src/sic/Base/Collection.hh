#ifndef SIC_COLLECTION_H_
#define SIC_COLLECTION_H_

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

private:
	const CollectionType &collection;

public:
	/**
	 * Construct a Collection wrapper.
	 */
	Collection(const CollectionType &collection) : collection(collection) {}

	typename sic::Iterators<ItemType> getItems() const override {
		return sic::Iterators<ItemType>(collection);
	}

	std::size_t size() const override { return collection.size(); }
};

} // namespace sic

#endif // SIC_COLLECTION_H_
