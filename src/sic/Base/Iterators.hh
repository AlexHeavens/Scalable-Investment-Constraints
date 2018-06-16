#ifndef SIC_ITERATORS_H_
#define SIC_ITERATORS_H_

#include <memory>

#include <boost/range/detail/any_iterator.hpp>

namespace sic {

/**
 * An aggregated current and end Iterator for easy comparison.
 */
template <typename Item> class Iterators {

public:
	// A generic catch-all for most iterator types.
	typedef boost::range_detail::any_iterator<
		Item, boost::iterators::forward_traversal_tag, Item &, std::ptrdiff_t>
		It;

private:
	sic::Iterators<Item>::It currentIt;
	sic::Iterators<Item>::It endIt;

public:
	/**
	 * Create an Iterator pair.
	 *
	 * @param collection Collection to iterator over.
	 */
	template <typename CollectionType>
	Iterators(const CollectionType &collection)
		: currentIt(collection.begin()), endIt(collection.end()) {}

	/**
	 * Create an Iterator pair.
	 *
	 * @param collection Collection to iterator over.
	 */
	template <typename CollectionType>
	Iterators(const std::unique_ptr<CollectionType> &collection)
		: currentIt(collection->begin()), endIt(collection->end()) {}

	/**
	 * Assigment to an Iterators.
	 */
	Iterators<Item> &operator=(const Iterators<Item> &lhs) {
		this->currentIt = lhs.currentIt;
		this->endIt = lhs.endIt;
		return *this;
	}

	/**
	 * The (modifiable) current iterator.
	 */
	sic::Iterators<Item>::It &current() { return currentIt; }

	/**
	 * The (fixed) end iterator.
	 */
	const sic::Iterators<Item>::It &end() { return endIt; }

	/**
	 * If there are remaining items to iterator through.
	 */
	bool remaining() const { return currentIt != endIt; }
};

} // namespace sic

#endif // SIC_ITERATORS_H_
