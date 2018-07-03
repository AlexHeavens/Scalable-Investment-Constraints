#ifndef SIC_SOURCE_H_
#define SIC_SOURCE_H_

#include "sic/Base/Iterators.hh"

namespace sic {

/**
 * Defines a generic sources of a given class type.
 */
template <typename ItemType> class Source {

public:
	/**
	 * Retrieve iterators for the items of type ItemType.
	 */
	virtual typename sic::Iterators<ItemType> getItems() const = 0;

	/**
	 * Number of items the Source provides.
	 */
	virtual std::size_t size() const = 0;

	/**
	 * End iterator.
	 */
	typename sic::Iterators<ItemType>::It end() const {
		return getItems().end();
	};
};

} // namespace sic

#endif // SIC_SOURCE_H_
