#ifndef SIC_ITERATORS_H_
#define SIC_ITERATORS_H_

namespace sic {

/**
 * An aggregated current and end Iterator for easy comparison.
 */
template <typename IteratorType> class Iterators {

private:
	IteratorType currentIt;
	const IteratorType endIt;

public:
	/**
	 * Create an Iterator pair.
	 */
	Iterators(IteratorType begin, const IteratorType end)
		: currentIt(begin), endIt(end) {}

	/**
	 * The (modifiable) current iterator.
	 */
	IteratorType &current() { return currentIt; }

	/**
	 * The (fixed) end iterator.
	 */
	const IteratorType &end() { return endIt; }
};

} // namespace sic

#endif // SIC_ITERATORS_H_
