#ifndef SIC_PORTFOLIO_H_
#define SIC_PORTFOLIO_H_

#include <memory>
#include <utility>
#include <vector>

#include "sic/Portfolio/Position.hh"

namespace sic {

/**
 * A client's portfolio, consisting of a list of Positions.
 *
 * @see sic::Position
 */
class Portfolio {
private:
	std::unique_ptr<std::vector<sic::Position>> positions;

public:
	/**
	 * Consruct a portfolio.
	 *
	 * @param positions a vector of positions that will be moved to the
	 * Portfolio.
	 */
	Portfolio(std::unique_ptr<std::vector<sic::Position>> positions)
		: positions(std::move(positions)) {}

	/**
	 * The begin and end iterators of the Portfolio's position vector.
	 *
	 * @returns Begin and end as the first and second sides of a pair.
	 */
	std::pair<sic::Position::VectorIterator, sic::Position::VectorIterator>
	getPositionIterators();
};

} // namespace sic

#endif // SIC_PORTFOLIO_H_
