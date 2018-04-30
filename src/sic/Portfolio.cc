#include "sic/Portfolio.hh"

std::pair<sic::Position::VectorIterator, sic::Position::VectorIterator>
sic::Portfolio::getPositionIterators() {

	return std::make_pair(std::begin(*positions), std::end(*positions));
}
