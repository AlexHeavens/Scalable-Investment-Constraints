#ifndef SIC_ABSTRACTPORTFOLIO_H_
#define SIC_ABSTRACTPORTFOLIO_H_

#include "sic/Base/Iterators.hh"
#include "sic/External.hh"
#include "sic/Portfolio/AbstractPosition.hh"

namespace sic {

/**
 * A client's portfolio, consisting of a list of Positions.
 *
 * @see sic::Position
 */
template <typename Position> class AbstractPortfolio : public sic::External {

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
	virtual sic::Iterators<Position> getPositionIterators() = 0;
};

} // namespace sic

#endif // SIC_ABSTRACTPORTFOLIO_H_
