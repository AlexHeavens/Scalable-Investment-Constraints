#include "sic/Portfolio.hh"

#include <stdexcept>
#include <unordered_set>

sic::Portfolio::Portfolio(std::unique_ptr<std::vector<sic::Position>> positions,
						  sic::External::ID externalID)
	: sic::AbstractPortfolio(externalID) {

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

sic::Iterators<sic::Position> sic::Portfolio::getPositionIterators() {

	sic::Iterators<sic::Position>::It begin(positions->begin());
	sic::Iterators<sic::Position>::It end(positions->end());

	return sic::Iterators<sic::Position>(begin, end);
}
