#ifndef SIC_RANGE_H_
#define SIC_RANGE_H_

namespace sic {

/**
 * Represent a min, target, max Range.
 */
template <typename Type> class Range {

public:
	const Type min;
	const Type target;
	const Type max;

	/**
	 * Create a min, target, max Range.
	 */
	Range(Type min, Type target, Type max)
		: min(min), target(target), max(max) {}
};

} // namespace sic

#endif // SIC_RANGE_H_
