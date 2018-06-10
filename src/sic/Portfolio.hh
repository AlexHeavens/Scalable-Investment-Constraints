#ifndef SIC_PORTFOLIO_H_
#define SIC_PORTFOLIO_H_

#include <memory>
#include <utility>
#include <vector>

#include "sic/Base/Iterators.hh"
#include "sic/External.hh"
#include "sic/Portfolio/Position.hh"

namespace sic {

/**
 * A client's portfolio, consisting of a list of Positions.
 *
 * @see sic::Position
 */
class Portfolio : public sic::External {
private:
	std::unique_ptr<std::vector<sic::Position>> positions;

public:
	/**
	 * Consruct a portfolio.
	 *
	 * @param positions a vector of positions that will be moved to the
	 * Portfolio.
	 *
	 * @throws invalid_argument if input positions have a duplicate external ID.
	 */
	Portfolio(std::unique_ptr<std::vector<sic::Position>> positions,
			  sic::External::ID externalID);

	/**
	 * The current and end iterators of the Portfolio's position vector.
	 *
	 * @returns Current and end iterators.
	 */
	sic::Iterators<sic::Position> getPositionIterators();
};

} // namespace sic

#endif // SIC_PORTFOLIO_H_
