#ifndef SIC_ABSTRACTPORTFOLIO_H_
#define SIC_ABSTRACTPORTFOLIO_H_

#include "sic/Base/Iterators.hh"
#include "sic/External.hh"

namespace sic {

class AbstractPortfolio;
class AbstractAssetAllocation;

} // namespace sic

#include "sic/Model/AbstractAssetAllocation.hh"
#include "sic/Portfolio/AbstractPosition.hh"

namespace sic {

/**
 * A client's portfolio, consisting of a list of Positions.
 *
 * @see sic::Position
 */
class AbstractPortfolio : public sic::External {

public:
	/**
	 * Root constructor for any Portfolio.
	 *
	 * @param externalID ID of the Portfolio in the external system.  Optional
	 * for ease of mock testing.
	 */
	AbstractPortfolio(sic::External::ID externalID = 0)
		: sic::External(externalID) {}

	/**
	 * Destroy an AbstractPortfolio.
	 */
	virtual ~AbstractPortfolio() {}

	/**
	 * The current and end iterators of the Portfolio's position vector.
	 *
	 * @returns Current and end iterators.
	 */
	virtual sic::Iterators<sic::AbstractPosition> getPositionIterators() = 0;

	/**
	 * The current and end iterators of the Portfolio's position vector.
	 *
	 * @returns Current and end iterators.
	 */
	virtual sic::Iterators<sic::AbstractPosition>
	getPositionIterators() const = 0;

	/**
	 * The summed reference value of the positions.
	 */
	virtual sic::Value getTotalReferenceValue() const = 0;

	/**
	 *  The number of Positions the Portfolio holds.
	 */
	virtual std::size_t getPositionCount() const = 0;

	/**
	 * Retrieve the AssetAllocations the Portfolio must follow.
	 */
	virtual sic::Iterators<const sic::AbstractAssetAllocation *>
	getAssetAllocations() const = 0;
};

} // namespace sic

#endif // SIC_ABSTRACTPORTFOLIO_H_
