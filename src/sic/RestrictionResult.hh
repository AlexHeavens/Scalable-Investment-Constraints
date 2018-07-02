#ifndef SIC_RESTRICTION_RESULT_H_
#define SIC_RESTRICTION_RESULT_H_

namespace sic {

/**
 * Interface for RestrictionResults.
 */
class RestrictionResult {

protected:
	RestrictionResult() = default;
	virtual ~RestrictionResult() = default;

public:
	/**
	 * Produce a string representation of the Result.
	 */
	virtual std::string serialise() const = 0;
};

} // namespace sic

#endif // SIC_RESTRICTION_RESULT_H_
