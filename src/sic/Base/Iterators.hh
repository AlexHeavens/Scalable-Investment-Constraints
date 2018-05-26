#ifndef SIC_ITERATORS_H_
#define SIC_ITERATORS_H_

#include <memory>

namespace sic {

/**
 * An aggregated current and end Iterator for easy comparison.
 */
template <typename IteratorType> class Iterators {

private:
	std::unique_ptr<IteratorType> currentIt;
	std::unique_ptr<const IteratorType> endIt;

public:
	/**
	 * Create an Iterator pair.
	 */
	Iterators(IteratorType begin, const IteratorType end) {

		IteratorType *currentPtr = new IteratorType;
		*currentPtr = begin;
		currentIt = std::unique_ptr<IteratorType>(currentPtr);

		IteratorType *endPtr = new IteratorType;
		*endPtr = end;
		endIt = std::unique_ptr<const IteratorType>(endPtr);
	}

	/**
	 * The (modifiable) current iterator.
	 */
	IteratorType &current() { return *currentIt; }

	/**
	 * The (fixed) end iterator.
	 */
	const IteratorType &end() { return *endIt; }
};

} // namespace sic

#endif // SIC_ITERATORS_H_
