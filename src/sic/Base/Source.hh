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
	 * Item at a given index of the Source,
	 *
	 * <b>Note:</b> by default this is implemented as a naive O(N) iteration. It
	 * should be overriden where the underlying collection type allows direct
	 * index access.
	 */
	virtual ItemType &at(const std::size_t &index) const {
		return *std::next(begin(), index);
	}

	/**
	 * Begin iterator.
	 */
	typename sic::Iterators<ItemType>::It begin() const {
		return getItems().begin();
	}

	/**
	 * End iterator.
	 */
	typename sic::Iterators<ItemType>::It end() const {
		return getItems().end();
	};
};

} // namespace sic

#endif // SIC_SOURCE_H_
