#ifndef SIC_RANGE_H_
#define SIC_RANGE_H_

#include <stdexcept>

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
		: min(min), target(target), max(max) {

		if (min > target) {
			throw std::invalid_argument(
				"Range min must be less or equal to target.");
		}
		if (max < target) {
			throw std::invalid_argument(
				"Range max must be greater or equal to target.");
		}
	}

	/**
	 * Create a range where the min, target and max are the same.
	 */
	explicit Range(Type target) : min(target), target(target), max(target) {}

	bool operator==(const Range<Type> &rhs) const {
		return this->min == rhs.min and this->target == rhs.target and
			   this->max == rhs.max;
	}
};

} // namespace sic

#endif // SIC_RANGE_H_
