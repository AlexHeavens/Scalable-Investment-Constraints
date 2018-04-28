#include "sic/Portfolio/Asset.hh"

std::pair<sic::Asset::ClassIterator, sic::Asset::ClassIterator>
sic::Asset::getClassIterators() {

	return std::make_pair(std::begin(classes), std::end(classes));
}
