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
	const sic::Iterators<Item>::It beginIt;
	sic::Iterators<Item>::It currentIt;
	const sic::Iterators<Item>::It endIt;

public:
	/**
	 * Create an Iterator pair from collection.
	 *
	 * @param collection Collection to iterator over.
	 */
	template <typename CollectionType>
	explicit Iterators(const CollectionType &collection)
		: beginIt(collection.begin()), currentIt(collection.begin()),
		  endIt(collection.end()) {}

	/**
	 * Create an Iterator pair from collection.
	 *
	 * @param collection Collection to iterator over.
	 */
	template <typename CollectionType>
	explicit Iterators(const std::unique_ptr<CollectionType> &collection)
		: beginIt(collection->begin()), currentIt(collection->begin()),
		  endIt(collection->end()) {}

	/**
	 * Create an Iterator pair from range  iterators.
	 */
	template <typename CollectionType>
	explicit Iterators(std::pair<typename CollectionType::iterator,
								 typename CollectionType::iterator>
						   range)
		: beginIt(range.first), currentIt(range.first), endIt(range.second) {}

	/**
	 * Copy construct Iterators.
	 */
	Iterators(const Iterators<Item> &other)
		: beginIt(other.beginIt), currentIt(other.currentIt),
		  endIt(other.endIt) {}

	/**
	 * Copy construct Iterators.
	 */
	Iterators(Iterators<Item> &other)
		: beginIt(other.beginIt), currentIt(other.currentIt),
		  endIt(other.endIt) {}

	/**
	 * Compare equivalence to another Iterators.
	 */
	bool operator==(const sic::Iterators<Item> &other) const {
		return beginIt == other.beginIt and currentIt == other.currentIt and
			   endIt == other.endIt;
	}

	/**
	 * The (fixed) begin iterator.
	 */
	const sic::Iterators<Item>::It &begin() const { return beginIt; }

	/**
	 * The (modifiable) current iterator.
	 */
	sic::Iterators<Item>::It &current() { return currentIt; }

	/**
	 * The (fixed) end iterator.
	 */
	const sic::Iterators<Item>::It &end() const { return endIt; }

	/**
	 * If there are remaining items to iterator through.
	 */
	bool remaining() const { return currentIt != endIt; }

	/**
	 * Reset current iterator to the initial (begin) iterator.
	 */
	void reset() { currentIt = beginIt; }
};

} // namespace sic

#endif // SIC_ITERATORS_H_
