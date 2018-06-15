#ifndef SIC_PORTFOLIO_H_
#define SIC_PORTFOLIO_H_

#include <memory>
#include <stdexcept>
#include <unordered_set>
#include <vector>

#include "sic/AbstractPortfolio.hh"
#include "sic/Portfolio/Position.hh"

namespace sic {

/**
 * A client's portfolio, consisting of a list of Positions.
 *
 * @see sic::Position
 */
template <typename Position = sic::Position>
class Portfolio : public sic::AbstractPortfolio<Position> {
private:
	std::unique_ptr<std::vector<Position>> positions;

public:
	/**
	 * Construct a portfolio.
	 *
	 * @param positions a vector of positions that will be moved to the
	 * Portfolio.
	 *
	 * @throws invalid_argument if input positions have a duplicate external ID.
	 */
	Portfolio(std::unique_ptr<std::vector<Position>> positions,
			  sic::External::ID externalID)
		: sic::AbstractPortfolio<Position>(externalID) {

		// Throw exception if positions have duplicate external ID.
		std::unordered_set<sic::External::ID> externalIDSet;
		for (const auto &inputPosition : *positions) {

			const auto inputPositionID = inputPosition.getExternalID();
			const auto idLookup = externalIDSet.find(inputPositionID);
			if (idLookup != externalIDSet.end()) {
				throw std::invalid_argument("Duplicate Portfolio Position ID");
			}

			externalIDSet.insert(inputPositionID);
		}

		this->positions = std::move(positions);
	}

	sic::Iterators<Position> getPositionIterators() override {

		typename sic::Iterators<Position>::It begin(positions->begin());
		typename sic::Iterators<Position>::It end(positions->end());

		return sic::Iterators<Position>(begin, end);
	}
};

} // namespace sic

#endif // SIC_PORTFOLIO_H_
