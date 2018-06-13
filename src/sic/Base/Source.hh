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
	virtual typename sic::Iterators<ItemType> getItems() = 0;
};

} // namespace sic

#endif // SIC_SOURCE_H_
