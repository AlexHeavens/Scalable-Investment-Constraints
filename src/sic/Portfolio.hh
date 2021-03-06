#ifndef SIC_PORTFOLIO_H_
#define SIC_PORTFOLIO_H_

#include <memory>
#include <stdexcept>
#include <unordered_set>

#include "sic/AbstractPortfolio.hh"
#include "sic/Portfolio/Position.hh"

namespace sic {

/**
 * A client's portfolio, consisting of a list of Positions.
 *
 * @see sic::Position
 */
template <typename Position = sic::Position>
class Portfolio : public sic::AbstractPortfolio {

	static_assert(std::is_base_of<sic::AbstractPosition, Position>::value,
				  "Position not derived from sic::AbstractPosition");

public:
	using AAVector = std::vector<const sic::AbstractAssetAllocation *>;

private:
	std::unique_ptr<std::vector<Position>> positions;
	const sic::Value totalReferenceValue;
	std::unique_ptr<AAVector> aaVector;

	static sic::Value sumPositions(const std::vector<Position> &positions) {
		sic::Value sum = 0.0;
		for (auto &position : positions) {
			sum += position.getReferenceValue();
		}
		return sum;
	}

public:
	/**
	 * Construct a portfolio.
	 *
	 * @param positions a vector of positions that will be moved to the
	 * Portfolio.
	 * @param externalID ID of the Portfolio in the external system.
	 * @param aaVector the AAs the Portfolio must follow.
	 *
	 * @throws invalid_argument if input positions have a duplicate external ID.
	 */
	Portfolio(std::unique_ptr<std::vector<Position>> positions,
			  sic::External::ID externalID,
			  std::unique_ptr<AAVector> aaVector = {})
		: sic::AbstractPortfolio(externalID),
		  totalReferenceValue(sumPositions(*positions)),
		  aaVector(std::move(aaVector)) {

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

	sic::Iterators<sic::AbstractPosition> getPositionIterators() override {
		return sic::Iterators<sic::AbstractPosition>(positions);
	}

	sic::Iterators<sic::AbstractPosition>
	getPositionIterators() const override {
		return sic::Iterators<sic::AbstractPosition>(positions);
	}

	sic::Value getTotalReferenceValue() const override {
		return totalReferenceValue;
	}

	std::size_t getPositionCount() const override { return positions->size(); }

	sic::Iterators<const sic::AbstractAssetAllocation *>
	getAssetAllocations() const override {
		return sic::Iterators<const sic::AbstractAssetAllocation *>(aaVector);
	}
};

} // namespace sic

#endif // SIC_PORTFOLIO_H_
