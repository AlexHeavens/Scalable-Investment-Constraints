#ifndef SIC_ITERATORS_H_
#define SIC_ITERATORS_H_

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
	const sic::Iterators<Item>::It endIt;

public:
	/**
	 * Create an Iterator pair.
	 */
	Iterators(sic::Iterators<Item>::It &begin,
			  const sic::Iterators<Item>::It &end)
		: currentIt(begin), endIt(end) {}

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
