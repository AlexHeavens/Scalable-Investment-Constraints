#ifndef SIC_PORTFOLIO_H_
#define SIC_PORTFOLIO_H_

#include <memory>
#include <vector>

#include "sic/AbstractPortfolio.hh"

namespace sic {

/**
 * A client's portfolio, consisting of a list of Positions.
 *
 * @see sic::Position
 */
class Portfolio : public sic::AbstractPortfolio {
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

	sic::Iterators<sic::Position> getPositionIterators() override;
};

} // namespace sic

#endif // SIC_PORTFOLIO_H_
